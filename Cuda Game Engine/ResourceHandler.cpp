#include "ResourceHandler.h"


ResourceHandler::ResourceHandler(void) :
    meshes(std::vector<ObjFileReader>()),
    textures(std::vector<Texture>()),
    shader_programs(std::vector<Shader>()),
    memory_objects(std::vector<BufferManager*>()),
    mesh_map(std::hash_map<std::string, ObjFileReader*>()),
    shader_map(std::hash_map<std::string, Shader*>()),
    texture_map(std::hash_map<std::string, Texture*>())
{
}


ResourceHandler::~ResourceHandler(void)
{
}

void ResourceHandler::loadMesh(std::string filepath)
{
    mesh_map.insert( std::pair<std::string, ObjFileReader*>(filepath, new ObjFileReader(filepath)) );
    memory_map.insert( std::pair<std::string, BufferManager*>(filepath, (mesh_map[filepath]->getBuffers()) ) );
	std::string s = memory_map[filepath]->getType();
}



void ResourceHandler::loadShader(std::string vs_path, std::string fs_path)
{
    shader_map.insert( std::pair<std::string, Shader*>(vs_path.append(fs_path), new Shader(vs_path, fs_path) ) );
}


//Returns an object associated with a certain mesh/shader/texture
ObjectState ResourceHandler::synthesizeObject(std::string shader_key, std::string mesh_key, std::string tex_key)
{
    Texture* tex = nullptr;
    Shader* shdr = nullptr;
    BufferManager* vbo = nullptr;
    if (memory_map[mesh_key])
        vbo = memory_map[mesh_key];

    if (shader_map[shader_key])
        shdr = shader_map[shader_key];

    if (texture_map[tex_key])
        tex = texture_map[tex_key];
    ObjectState to_return = ObjectState(vbo, shdr, tex);

    return to_return;
}

Texture* ResourceHandler::loadTexture(std::string img_filepath)
{
    texture_map.insert(std::pair<std::string, Texture*>( img_filepath, new Texture(img_filepath) ) );
    return texture_map[img_filepath];
}

Texture* ResourceHandler::loadTexture(std::string img_filepath, int w, int h)
{
    texture_map.insert(std::pair<std::string, Texture*>( img_filepath, new Texture(img_filepath, w, h) ) );
    return texture_map[img_filepath];
}

Texture* ResourceHandler::loadCompressedTexture(std::string img_filepath)
{
    texture_map.insert(std::pair<std::string, Texture*>( img_filepath, new CompressedTexture(img_filepath) ) );
    return texture_map[img_filepath];
}

Texture3D* ResourceHandler::load3DTexture(std::string img_filepath)
{
    Texture3D* ptr = new Texture3D(img_filepath);
    texture_map.insert(std::pair<std::string, Texture3D*>( img_filepath, ptr ) );
    return ptr;
}

Texture* ResourceHandler::loadMipmapTexture(std::string img_filepath)
{
    texture_map.insert(std::pair<std::string, Texture*>( img_filepath, new MipmappedTexture(img_filepath) ) );
    return texture_map[img_filepath];
}

FrameBufferTexture* ResourceHandler::loadFramebufferTexture(int left, int bottom, int width, int height, std::string name)
{
    FrameBufferTexture* to_return = new FrameBufferTexture(left, bottom, width, height);
    texture_map.insert(std::pair<std::string, FrameBufferTexture*>( name,  to_return) );
    return to_return;
}

//Returns Shader where filepath key is the 'file location of VS'.append('FS file location')
Shader* ResourceHandler::getShaderByKey(std::string filepath_key)
{
    Shader* to_return = nullptr;
    if (shader_map[filepath_key])
        to_return = shader_map[filepath_key];
    else
    {
        std::cout << "Invalid paths to identify shader" << std::endl;
        std::cout << "Shaders pairs are loaded from a resource .xml file." << std::endl;
        std::cout << "The key for a shader pair is 'path to VS''path to FS'" << std::endl;
        std::cout << "Check the resource .xml file to ensure that the pair is loaded together." << std::endl;
    }

    return to_return;
}

//Returns Buffers read from the .obj file at filepath_key
BufferManager* ResourceHandler::getBufferByKey(std::string filepath_key)
{
    BufferManager* to_return = nullptr;
    if (memory_map[filepath_key])
	{
        to_return = memory_map[filepath_key];
	}
    else
        std::cout << "Invalid buffer key" << std::endl;

    return to_return;
}

//Returns Texture read from the file at filepath_key
Texture* ResourceHandler::getTextureByKey(std::string filepath_key)
{
    Texture* to_return = nullptr;
    if (texture_map[filepath_key])
        to_return = texture_map[filepath_key];
    else
        std::cout << "Invalid texture key" << std::endl;

    return to_return;
}

BufferManager* ResourceHandler::loadTransformBuffer(std::string filepath)
{
    BufferManager* to_add = new TransformFeedbackBuffer(filepath);
    memory_objects.push_back(to_add);
    return to_add;
}