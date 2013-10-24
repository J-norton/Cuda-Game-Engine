#include "Visitor.h"

ResourceHandler Visitor::scene_objects = ResourceHandler();

Visitor::Visitor(void):
    read_data(false)
{
}


Visitor::~Visitor(void)
{
}

void Visitor::visitCompositeGrid(CompositeGrid* tiles)
{
    if (!tiles->is_leaf)
        for (int i = 0; i < tiles->children.size(); i++)
        {
            CompositeGrid c = *tiles->children[i];
            c.acceptVisitor(this);
        }
}

void Visitor::readData()
{
    pugi::xml_parse_result res = doc.load_file(resource_path.c_str()); 
    if (!res)
        std::cout << "Error, could not open resource XML file: "<< std::endl << resource_path << std::endl;

    for (pugi::xml_node_iterator current = doc.root().begin(); current != doc.root().end(); ++current)
    {
        for (pugi::xml_node_iterator resource = current->begin(); resource != current->end(); ++resource)
        {
            if (std::strcmp(resource->name(), "Resource") == 0)
            {
                pugi::xml_attribute path = resource->attribute("ObjFilePath");
                std::string obj_path = path.as_string();
                scene_objects.loadMesh(obj_path);
            }
            else if (std::strcmp(resource->name(), "ShaderPair") == 0)
            {
                pugi::xml_attribute vs_path = resource->attribute("VertexShaderPath");
                pugi::xml_attribute fs_path = resource->attribute("FragmentShaderPath");

                scene_objects.loadShader(vs_path.as_string(), fs_path.as_string());
            }
            else
                loadTextureFromXml(*resource);
        }
    }
    read_data = true;
    //file_handler.parseDataFromDirectory(&scene_objects);
}

//Creates and initializes a texture object from xml
Texture* Visitor::loadTextureFromXml(pugi::xml_node& xml_text_data)
{
    Texture* to_return = nullptr;
    if (std::strcmp(xml_text_data.name(), "Texture") == 0)
    {
        pugi::xml_attribute tex_path = xml_text_data.attribute("TexturePath");
        to_return = scene_objects.loadTexture(tex_path.as_string());
        if (glGetError() != 0)
            std::cout << "Illegal arguments set for texture";
        for (pugi::xml_node_iterator tex_parameter = xml_text_data.begin(); tex_parameter != xml_text_data.end(); ++tex_parameter)
            setGLCodeFromXml(*tex_parameter, to_return);
    }
    else if (std::strcmp(xml_text_data.name(), "CompressedTexture") == 0)
    {
        pugi::xml_attribute tex_path = xml_text_data.attribute("TexturePath");
        to_return = scene_objects.loadCompressedTexture(tex_path.as_string());
        for (pugi::xml_node_iterator tex_parameter = xml_text_data.begin(); tex_parameter != xml_text_data.end(); ++tex_parameter)
            setGLCodeFromXml(*tex_parameter, to_return);
    }
    else if (std::strcmp(xml_text_data.name(), "Texture3D") == 0)
    {
        pugi::xml_attribute tex_path = xml_text_data.attribute("TexturePath");
        to_return = scene_objects.load3DTexture(tex_path.as_string());
        for (pugi::xml_node_iterator tex_parameter = xml_text_data.begin(); tex_parameter != xml_text_data.end(); ++tex_parameter)
            setGLCodeFromXml(*tex_parameter, to_return);
    }
    else if (std::strcmp(xml_text_data.name(), "MipmapTexture") == 0)
    {
        pugi::xml_attribute tex_path = xml_text_data.attribute("TexturePath");
        to_return = scene_objects.loadMipmapTexture(tex_path.as_string());
        for (pugi::xml_node_iterator tex_parameter = xml_text_data.begin(); tex_parameter != xml_text_data.end(); ++tex_parameter)
            setGLCodeFromXml(*tex_parameter, to_return);
    }
    else if (std::strcmp(xml_text_data.name(), "FrameBufferTexture") == 0)
    {
        int left = xml_text_data.attribute("left").as_int();
        int bottom = xml_text_data.attribute("Bottom").as_int();
        int width = xml_text_data.attribute("Width").as_int();
        int height = xml_text_data.attribute("Height").as_int();
        std::string name = xml_text_data.attribute("Name").as_string();

        to_return = scene_objects.loadFramebufferTexture(left, bottom, width, height, name);

        for (pugi::xml_node_iterator tex_parameter = xml_text_data.begin(); tex_parameter != xml_text_data.end(); ++tex_parameter)
            setGLCodeFromXml(*tex_parameter, to_return);
        for (pugi::xml_node_iterator tex_parameter = xml_text_data.begin(); tex_parameter != xml_text_data.end(); ++tex_parameter)
        {
            if (strcmp(tex_parameter->name(), "TriangleMesh") == 0)
                (static_cast<FrameBufferTexture*>(to_return))->addMesh(loadMeshFromXml(*tex_parameter));
        }
    }
    else
        std::cout << "Undefined XML node type, check resource file for errors." << std::endl;

    return to_return;
}

//Creates a TriangleMesh instance from provided xml data
TriangleMesh Visitor::loadMeshFromXml(pugi::xml_node& resource)
{

    Shader* mesh_shader = nullptr;
    Texture* mesh_texture = nullptr;
    BufferManager* mesh_buffers = nullptr;
    pugi::xml_node scenario_texture = resource.child("Texture");

    if (resource.child("Texture") || resource.child("Texture3D") || resource.child("FrameBufferTexture") ||
        resource.child("CompressedTexture") || resource.child("MipmapTexture") )
        for (pugi::xml_node_iterator textures = resource.begin(); textures != resource.end(); ++textures)
        {
            mesh_texture = loadTextureFromXml(*textures);
            if (mesh_texture != nullptr)
                break;
        }
    else
        mesh_texture = scene_objects.getTextureByKey(resource.attribute("Texture").as_string());

    std::string fs_path = resource.attribute("FSPath").as_string();
    std::string vs_path = resource.attribute("VSPath").as_string();
    mesh_shader = scene_objects.getShaderByKey(vs_path.append(fs_path));

    std::string obj_path = resource.attribute("ObjFile").as_string();
    mesh_buffers = scene_objects.getBufferByKey(obj_path);
    pugi::xml_attribute instance = resource.attribute("InstanceCount");

    ObjectState scenario_state = ObjectState(mesh_buffers, mesh_shader, mesh_texture);

    if (resource.child("TransformFeedbackOutput") )
    {
        pugi::xml_node feedback_node = resource.child("TransformFeedbackOutput");
        std::string transform_name = feedback_node.attribute("Name").as_string();
        std::string vs_path = feedback_node.attribute("ShaderPath").as_string();
        mesh_shader->addTransformedOutput(transform_name);

        BufferManager* new_trans_buffer = scene_objects.loadTransformBuffer(vs_path);
        ShaderParameter transform_param = ShaderParameter(transform_name, 0, 4, nullptr, false);
        new_trans_buffer->addBuffer(transform_param);
        scenario_state.addTransformBuffer( new_trans_buffer );
    }


    //Now loop over all nodes to determine which blocks the shader should bind to 
    for (pugi::xml_node_iterator uniforms = resource.begin(); uniforms != resource.end(); ++uniforms)
    {
        if (std::strcmp(uniforms->name(), "ShaderUniforms") == 0)
        {
            std::string block_name = uniforms->attribute("Name").as_string();
            int block_index = uniforms->attribute("BindingPoint").as_int();
            scenario_state.addUniformPair(std::pair<std::string, int>(block_name, block_index));
        }
    }

    //Now gather all information for uniforms to the shaders
    for (pugi::xml_node_iterator uniforms = resource.begin(); uniforms != resource.end(); ++uniforms)
    {
        if (std::strcmp(uniforms->name(), "UniformBlock") == 0)
        {
            UniformBlock to_add = synthesizeBlock(*uniforms);
            mesh_shader->addUniformBlock(to_add);
        }
        if (std::strcmp(uniforms->name(), "Uniform") == 0)
        {
            ShaderParameter uniform = parseUniform(*uniforms);
            mesh_shader->addUniform(uniform);
        }
    }
    if (instance)
        return TriangleMesh(scenario_state, instance.as_int());
    else
        return TriangleMesh(scenario_state);
}


//Parses XML data into calls to glTexParameteri
void Visitor::setGLCodeFromXml(pugi::xml_node xml_input, Texture* texture)
{
    if (std::strcmp(xml_input.name(), "Parameter") == 0)
    {
        for (pugi::xml_attribute_iterator tex_parameter = xml_input.attributes_begin();
            tex_parameter != xml_input.attributes_end(); ++tex_parameter)
        {
            //Signals whether the parameter passed to openGL is a macro or integer
            bool expect_int = false;
            //Signals whether the parameter passed to openGL is a float or an integer
            bool expect_float = false;
            std::string param_id = tex_parameter->name();
            std::string param_value = tex_parameter->as_string();
            int param_type = 0;

            //Filtering options
            if (strcmp(param_id.c_str(), "MinFilter") == 0)
                param_type = GL_TEXTURE_MIN_FILTER;
            else if (strcmp(param_id.c_str(), "MagFilter") == 0)
                param_type = GL_TEXTURE_MAG_FILTER;

            // Min/Max values for texture
            else if (strcmp(param_id.c_str(), "MaxLOD") == 0)
            {
                param_type = GL_TEXTURE_MAX_LOD;
                expect_float = true;
            }
            else if (strcmp(param_id.c_str(), "MinLOD") == 0)
            {
                param_type = GL_TEXTURE_MIN_LOD;
                expect_float = true;
            }
            else if (strcmp(param_id.c_str(), "BaseLevel") == 0)
            {
                param_type = GL_TEXTURE_BASE_LEVEL;
                expect_int = true;
            }
            else if (strcmp(param_id.c_str(), "MaxLevel") == 0)
            {
                param_type = GL_TEXTURE_MAX_LEVEL;
                expect_int = true;
            }

            //Comparison options
            else if (strcmp(param_id.c_str(), "CompareMode") == 0)
                param_type = GL_TEXTURE_COMPARE_MODE;
            else if (strcmp(param_id.c_str(), "CompareFunc") == 0)
                param_type = GL_TEXTURE_COMPARE_FUNC;

            //Swizzle options
            else if (strcmp(param_id.c_str(), "SwizzleR") == 0)
                param_type = GL_TEXTURE_SWIZZLE_R;
            else if (strcmp(param_id.c_str(), "SwizzleG") == 0)
                param_type = GL_TEXTURE_SWIZZLE_G;
            else if (strcmp(param_id.c_str(), "SwizzleB") == 0)
                param_type = GL_TEXTURE_SWIZZLE_B;
            else if (strcmp(param_id.c_str(), "SwizzleA") == 0)
                param_type = GL_TEXTURE_SWIZZLE_A;

            //Texture wrapping modes
            else if (strcmp(param_id.c_str(), "WrapS") == 0)
                param_type = GL_TEXTURE_WRAP_S;
            else if (strcmp(param_id.c_str(), "WrapT") == 0)
                param_type = GL_TEXTURE_WRAP_T;
            else if (strcmp(param_id.c_str(), "WrapR") == 0)
                param_type = GL_TEXTURE_WRAP_R;
            else
            {
                //If we are here an unknown identifier was supplied
                std::cout << "Unknown parameter type: " << param_id << std::endl;
                std::cout << "Check XML Resource file for errors" << std::endl;
                return;
            }

            //Now need to discern the argument to pass
            int gl_parameter = 0;

            //Regular filter types
            if (strcmp(param_value.c_str(), "Linear") == 0)
                gl_parameter = GL_LINEAR;
            else if (strcmp(param_value.c_str(), "Nearest") == 0)
                gl_parameter = GL_NEAREST;

            //Mipmap filter types
            else if (strcmp(param_value.c_str(), "NearestMipmapNearest") == 0)
                gl_parameter = GL_NEAREST_MIPMAP_NEAREST;
            else if (strcmp(param_value.c_str(), "NearestMipmapLinear") == 0)
                gl_parameter = GL_NEAREST_MIPMAP_LINEAR;
            else if (strcmp(param_value.c_str(), "LinearMipmapNearest") == 0)
                gl_parameter = GL_LINEAR_MIPMAP_NEAREST;
            else if (strcmp(param_value.c_str(), "LinearMipmapLinear") == 0)
                gl_parameter = GL_LINEAR_MIPMAP_LINEAR;

            //Comparison functions
            else if (strcmp(param_value.c_str(), "CompareRefToTexture") == 0)
                gl_parameter = GL_COMPARE_REF_TO_TEXTURE;
            else if (strcmp(param_value.c_str(), "None") == 0)
                gl_parameter = GL_NONE;

            //Colors
            else if (strcmp(param_value.c_str(), "Red") == 0)
                gl_parameter = GL_RED;
            else if (strcmp(param_value.c_str(), "Green") == 0)
                gl_parameter = GL_GREEN;
            else if (strcmp(param_value.c_str(), "Blue") == 0)
                gl_parameter = GL_BLUE;
            else if (strcmp(param_value.c_str(), "Alpha") == 0)
                gl_parameter = GL_ALPHA;
            else if (strcmp(param_value.c_str(), "One") == 0)
                gl_parameter = GL_ONE;
            else if (strcmp(param_value.c_str(), "Zero") == 0)
                gl_parameter = GL_ZERO;

            //Wrapping modes
            else if (strcmp(param_value.c_str(), "ClampToEdge") == 0)
                gl_parameter = GL_CLAMP_TO_EDGE;
            else if (strcmp(param_value.c_str(), "MirroredRepeat") == 0)
                gl_parameter = GL_MIRRORED_REPEAT;
            else if (strcmp(param_value.c_str(), "Repeat") == 0)
                gl_parameter = GL_REPEAT;

            //Function options
            else if (strcmp(param_value.c_str(), "LEqual") == 0)
                gl_parameter = GL_LEQUAL;
            else if (strcmp(param_value.c_str(), "GEqual") == 0)
                gl_parameter = GL_GEQUAL;
            else if (strcmp(param_value.c_str(), "GLess") == 0)
                gl_parameter = GL_LESS;
            else if (strcmp(param_value.c_str(), "Greater") == 0)
                gl_parameter = GL_GREATER;
            else if (strcmp(param_value.c_str(), "Equal") == 0)
                gl_parameter = GL_EQUAL;
            else if (strcmp(param_value.c_str(), "Notequal") == 0)
                gl_parameter = GL_NOTEQUAL;
            else if (strcmp(param_value.c_str(), "Always") == 0)
                gl_parameter = GL_ALWAYS;
            else if (strcmp(param_value.c_str(), "Never") == 0)
                gl_parameter = GL_NEVER;

            //Check if we are expecting an int or float
            else if (expect_int)
                gl_parameter = atoi(param_value.c_str());
            else if (expect_float)
                gl_parameter = atof(param_value.c_str());
            else
            {
                //If we are here an unknown identifier was supplied
                std::cout << "Unknown parameter value: " << param_value << std::endl;
                std::cout << "Check XML Resource file for errors" << std::endl;
                return;
            }

            if (expect_float)
                texture->setFloatParameter(param_type, gl_parameter);
            else
                texture->setIntParameter(param_type, gl_parameter);
        }
    }
}

ShaderParameter Visitor::parseUniform(pugi::xml_node& xml_data)
{
        std::string uniform_name = xml_data.attribute("Name").as_string();
        //Keep track of the running total size in bytes of the uniform block
        int block_size = 0;

        std::string type = xml_data.attribute("Type").as_string();
        float* uniform_data;
        if (strcmp(type.c_str(), "float") == 0)
        {
            block_size = 1;
            float data_str = xml_data.attribute("Data0").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str;
        }
        else if (strcmp(type.c_str(), "vec2") == 0)
        {
            block_size = 2;
            float data_str = xml_data.attribute("Data0").as_float();
            float data_str1 = xml_data.attribute("Data1").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str;
            uniform_data[1] = data_str1;
        }
        else if (strcmp(type.c_str(), "vec3") == 0)
        {
            block_size = 3;
            float data_str0 = xml_data.attribute("Data0").as_float();
            float data_str1 = xml_data.attribute("Data1").as_float();
            float data_str2 = xml_data.attribute("Data2").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str0;
            uniform_data[1] = data_str1;
            uniform_data[2] = data_str2;
        }
        else if (strcmp(type.c_str(), "vec4") == 0)
        {
            block_size = 4;
            float data_str0 = xml_data.attribute("Data0").as_float();
            float data_str1 = xml_data.attribute("Data1").as_float();
            float data_str2 = xml_data.attribute("Data2").as_float();
            float data_str3 = xml_data.attribute("Data3").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str0;
            uniform_data[1] = data_str1;
            uniform_data[2] = data_str2;
            uniform_data[3] = data_str3;
        }
        //TODO: Check for other types

        ShaderParameter next_uniform = ShaderParameter(uniform_name, block_size * 4, block_size, uniform_data, false);
		return next_uniform;
}

UniformBlock Visitor::synthesizeBlock(pugi::xml_node xml_data)
{
    int binding_pt = xml_data.attribute("BindingPoint").as_int();

    UniformBlock to_add = UniformBlock(xml_data.attribute("Name").as_string(), binding_pt);

    for (pugi::xml_node_iterator uniform = xml_data.begin(); uniform != xml_data.end(); ++uniform)
    {
        std::string uniform_name = uniform->attribute("Name").as_string();
        //Keep track of the running total size in bytes of the uniform block
        int block_size = 0;

        std::string type = uniform->attribute("Type").as_string();
        float* uniform_data;
        if (strcmp(type.c_str(), "float") == 0)
        {
            block_size = 1;
            float data_str = uniform->attribute("Data0").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str;
        }
        else if (strcmp(type.c_str(), "vec2") == 0)
        {
            block_size = 2;
            float data_str = uniform->attribute("Data0").as_float();
            float data_str1 = uniform->attribute("Data1").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str;
            uniform_data[1] = data_str1;
        }
        else if (strcmp(type.c_str(), "vec3") == 0)
        {
            block_size = 3;
            float data_str0 = uniform->attribute("Data0").as_float();
            float data_str1 = uniform->attribute("Data1").as_float();
            float data_str2 = uniform->attribute("Data2").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str0;
            uniform_data[1] = data_str1;
            uniform_data[2] = data_str2;
        }
        else if (strcmp(type.c_str(), "vec4") == 0)
        {
            block_size = 4;
            float data_str0 = uniform->attribute("Data0").as_float();
            float data_str1 = uniform->attribute("Data1").as_float();
            float data_str2 = uniform->attribute("Data2").as_float();
            float data_str3 = uniform->attribute("Data3").as_float();
            uniform_data = new float[block_size];
            uniform_data[0] = data_str0;
            uniform_data[1] = data_str1;
            uniform_data[2] = data_str2;
            uniform_data[3] = data_str3;
        }
        //TODO: Check for other types

        ShaderParameter next_uniform = ShaderParameter(uniform_name, block_size * 4, block_size, uniform_data, true);
        to_add.addParameter(next_uniform);
    }
    return to_add;
}