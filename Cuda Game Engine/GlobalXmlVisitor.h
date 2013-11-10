#pragma once
#include "Visitor.h"

class GlobalXmlVisitor :
	public Visitor
{
private:
	int argc;
	char** argv;
	std::string scenario_path;
	void parseNode();
	void parseMeshesToNode();
	virtual void visitNode(Node* node, pugi::xml_node& xml_node);
public:
	//virtual void visitCompositeGrid(CompositeGrid* tiles);
	virtual void visitNode(Node* node);
	GlobalXmlVisitor(const GlobalXmlVisitor& copy);
	GlobalXmlVisitor(std::string file_path, std::string scenario_path, int c, char** v);
	GlobalXmlVisitor(std::string file_path, int c, char** v);
	GlobalXmlVisitor(void);
	~GlobalXmlVisitor(void);
};

