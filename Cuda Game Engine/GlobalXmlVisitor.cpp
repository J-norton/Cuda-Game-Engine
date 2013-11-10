#include "GlobalXmlVisitor.h"


GlobalXmlVisitor::GlobalXmlVisitor(std::string resource, std::string scenario, int c, char** v) :
	scenario_path(scenario),
	argc(c),
	argv(v)
{
	resource_path = resource;
}

GlobalXmlVisitor::GlobalXmlVisitor(std::string resource, int c, char** v) :
	argc(c),
	argv(v)
{
	resource_path = resource;
}

GlobalXmlVisitor::GlobalXmlVisitor(const GlobalXmlVisitor& copy) :
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

//void GlobalXmlVisitor::visitCompositeGrid(CompositeGrid* tiles)
//{
//if (!read_data)
//	readData();
//pugi::xml_parse_result res = doc.load_file(scenario_path.c_str()); 
//if (!res)
//	std::cout << "Errors in XML file\n" << scenario_path.c_str() << "\n";
//for (pugi::xml_node_iterator current = doc.root().begin(); current != doc.root().end(); ++current)
//{
//	for (pugi::xml_node_iterator resource = current->begin(); resource != current->end(); ++resource)
//	{
//		if (std::strcmp(resource->name(), "TriangleMesh") == 0)
//		{
//			TriangleMesh to_add = loadMeshFromXml(*resource);
//			tiles->meshes.push_back( to_add );
//		}
//		if (std::strcmp(resource->name(), "Camera") == 0)
//		{
//			parseCamera(*resource, tiles);
//		}
//	}
//}
//}

void GlobalXmlVisitor::visitNode(Node* node)
{
	if (!read_data)
	{
		readData();
		pugi::xml_parse_result res = doc.load_file(scenario_path.c_str()); 
		if (!res)
			std::cout << "Errors in XML file\n" << scenario_path.c_str() << "\n";
	}
	for (pugi::xml_node_iterator current = doc.root().begin(); current != doc.root().end(); ++current)
	{
		for (pugi::xml_node_iterator resource = current->begin(); resource != current->end(); ++resource)
		{
			std::string debug = resource->name();
			if (std::strcmp(resource->name(), "TriangleMesh") == 0)
			{
				TriangleMesh to_add = loadMeshFromXml(*resource, *node);
				//node->meshes.push_back( to_add );
			}
			if (std::strcmp(resource->name(), "SpatialNode") == 0)
			{
				Node* next = new Node();
				visitNode(next, *resource);
				node->children.push_back(next);
				//node->children.push_back();
				//parseCamera(*resource, tiles);
			}
		}
	}
}

void GlobalXmlVisitor::visitNode(Node* node, pugi::xml_node& xml_node)
{
	//for (pugi::xml_node_iterator root_node = xml_node.root().begin(); root_node != xml_node.root().end(); ++root_node)
	//{
		for (pugi::xml_node_iterator current = xml_node.begin(); current != xml_node.end(); current++)
		{
			std::string st = current->name();
			if (std::strcmp(current->name(), "TriangleMesh") == 0)
			{
				TriangleMesh to_add = loadMeshFromXml(*current, *node);
				//node->meshes.push_back( to_add );
			}
			if (std::strcmp(current->name(), "SpatialNode") == 0)
			{
				//node->children.push_back();
				//parseCamera(*resource, tiles);
			}
		}
	//}
}