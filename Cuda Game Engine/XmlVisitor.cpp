#include "XmlVisitor.h"

XmlVisitor::XmlVisitor(std::string resource, int c, char** v) :
	argc(c),
	argv(v),
	file_handler (TestEngine(argc, "../Test Files/XML Scenarios") )
{
	resource_path = resource;
}

XmlVisitor::XmlVisitor(const XmlVisitor& copy) :
	file_handler(copy.file_handler)
{
	resource_path = copy.resource_path;
}


XmlVisitor::~XmlVisitor(void)
{
}

void XmlVisitor::visitCompositeGrid(CompositeGrid* tiles)
{
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

					std::string fs_path = resource->attribute("FSPath").as_string();
					std::string vs_path = resource->attribute("VSPath").as_string();
					std::string obj_path = resource->attribute("ObjFile").as_string();
					std::string tex_path = resource->attribute("Texture").as_string();
					pugi::xml_attribute test = resource->attribute("UniformBlock");
					std::string uniform_block = resource->attribute("UniformBlock").as_string();

					ObjectState scenario_state = scene_objects.synthesizeObject(vs_path.append(fs_path), obj_path, tex_path);
					UniformBlock to_add = UniformBlock(uniform_block, 0);
					float* model_view = new float[16];
					float* projection = new float[16];
					int k = 0;
					float l = 4.0 * (float) tiles->left / 512.0 - 4.0;
					float r = 4.0 * (float) tiles->right / 512.0 - 4.0;
					float b = 4.0 * (float) tiles->bottom / 512.0 - 4.0;
					float t = 4.0 * (float) tiles->top / 512.0 - 4.0;

					model_view[0] = 0.01;
					model_view[1] = 0;
					model_view[2] = 0;
					model_view[3] = 0;

					model_view[4] = 0;
					model_view[5] = 0.01;
					model_view[6] = 0;
					model_view[7] = 0;

					model_view[8] = 0;
					model_view[9] = 0;
					model_view[10] = 0.01;
					model_view[11] = 0;

					model_view[12] = (l + r) / 2;
					model_view[13] = (b + t) / 2;
					model_view[14] = -3.0;
					model_view[15] = 1;

					projection[0] = 1;
					projection[1] = 0;
					projection[2] = 0;
					projection[3] = 0;

					projection[4] = 0;
					projection[5] = 1;
					projection[6] = 0;
					projection[7] = 0;

					projection[8] = 0;
					projection[9] = 0;
					projection[10] = -11.0 / 9.0;
					projection[11] = -1;

					projection[12] = 0;
					projection[13] = 0;
					projection[14] = -20.0 / 9.0;
					projection[15] = 0;

					ShaderParameter param = ShaderParameter("ModelView", 16 * 4, 16, model_view, true);
					ShaderParameter param2 = ShaderParameter("Projection", 16 * 4, 16, projection, true);
					to_add.addParameter(param);
					to_add.addParameter(param2);
					scenario_state.addUniformBlock(to_add);
					tiles->meshes.push_back( TriangleMesh(scenario_state) );
				}
			}
		}
	}

	if (!tiles->is_leaf)
		for (CompositeGrid* child: tiles->children)
			child->acceptVisitor(this);
}
