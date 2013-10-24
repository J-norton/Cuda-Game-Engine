#include "GlobalXmlVisitor.h"

//ResourceHandler GlobalXmlVisitor::scene_objects = ResourceHandler();

GlobalXmlVisitor::GlobalXmlVisitor(std::string resource, std::string scenario, int c, char** v) :
    scenario_path(scenario),
    argc(c),
    argv(v),
    file_handler (TestEngine(argc, "../Test Files/XML Scenarios") )
{
    resource_path = resource;
    //file_handler.parseDataFromDirectory(&scene_objects);
}

GlobalXmlVisitor::GlobalXmlVisitor(std::string resource, int c, char** v) :
    argc(c),
    argv(v),
    file_handler (TestEngine(argc, "../Test Files/XML Scenarios") )
{
    resource_path = resource;
    //file_handler.parseDataFromDirectory(&scene_objects);
}

GlobalXmlVisitor::GlobalXmlVisitor(const GlobalXmlVisitor& copy) :
    file_handler(copy.file_handler),
    scenario_path(copy.scenario_path)
{
    resource_path = copy.resource_path;
}

GlobalXmlVisitor::GlobalXmlVisitor(void)
{
}


GlobalXmlVisitor::~GlobalXmlVisitor(void)
{
}

void GlobalXmlVisitor::visitCompositeGrid(CompositeGrid* tiles)
{
    if (!read_data)
        readData();
    pugi::xml_parse_result res = doc.load_file(scenario_path.c_str()); 
    if (!res)
        std::cout << "Errors in XML file\n" << scenario_path.c_str() << "\n";
    for (pugi::xml_node_iterator current = doc.root().begin(); current != doc.root().end(); ++current)
    {
        for (pugi::xml_node_iterator resource = current->begin(); resource != current->end(); ++resource)
        {
            if (std::strcmp(resource->name(), "TriangleMesh") == 0)
            {
                TriangleMesh to_add = loadMeshFromXml(*resource);
                tiles->meshes.push_back( to_add );
            }
            //if (std::strcmp(resource->name(), "UniformBlock") == 0)
            //{
            //    UniformBlock to_add = synthesizeBlock(*uniforms);
            //    mesh_shader->addUniformBlock(to_add);
            //}
        }

    }

    //ObjectState scenario_state = scene_objects.synthesizeObject("VS Texture.glslFS Texture.glsl",
    //    "Texture_Face.obj", "");
    //tiles->error_texture = TriangleMesh(scenario_state);
}

