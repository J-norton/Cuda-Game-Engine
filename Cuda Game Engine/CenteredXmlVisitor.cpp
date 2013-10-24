#include "CenteredXmlVisitor.h"


CenteredXmlVisitor::CenteredXmlVisitor(std::string resource, int c, char** v) :
    argc(c),
    argv(v),
    file_handler (TestEngine(argc, "../Test Files/XML Scenarios") )
{
    resource_path = resource;
    //file_handler.parseDataFromDirectory(&scene_objects);
}

CenteredXmlVisitor::CenteredXmlVisitor(const CenteredXmlVisitor& copy) :
    file_handler(copy.file_handler)
{
    resource_path = copy.resource_path;
}

CenteredXmlVisitor::CenteredXmlVisitor(void)
{
}


CenteredXmlVisitor::~CenteredXmlVisitor(void)
{
}



void CenteredXmlVisitor::visitCompositeGrid(CompositeGrid* tiles)
{
    if (!read_data)
        readData();
    if (tiles->is_leaf)
    {
        std::string current = file_handler.getNextPath();
        pugi::xml_parse_result res = doc.load_file(current.c_str()); 
        if (!res)
            std::cout << "Errors in XML file\n" << current.c_str() << "\n";
        for (pugi::xml_node_iterator current = doc.root().begin(); current != doc.root().end(); ++current)
        {
            for (pugi::xml_node_iterator resource = current->begin(); resource != current->end(); ++resource)
            {
                if (std::strcmp(resource->name(), "TriangleMesh") == 0)
                {
                    TriangleMesh to_add = loadMeshFromXml(*resource);
                    tiles->meshes.push_back( to_add );
                }
            }

        }

        ObjectState scenario_state = scene_objects.synthesizeObject("VS Texture.glslFS Texture.glsl",
            "Texture_Face.obj", "");
        tiles->error_texture = TriangleMesh(scenario_state);
    }

    if (!tiles->is_leaf)
        for (CompositeGrid* child: tiles->children)
            child->acceptVisitor(this);
}

