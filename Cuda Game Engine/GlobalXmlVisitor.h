#pragma once
#include "Visitor.h"
#include "ResourceHandler.h"

class GlobalXmlVisitor :
	public Visitor
{
private:
	int argc;
	char** argv;
	std::string scenario_path;
	TestEngine file_handler;
public:
	virtual void visitCompositeGrid(CompositeGrid* tiles);
	GlobalXmlVisitor(const GlobalXmlVisitor& copy);
	GlobalXmlVisitor(std::string file_path, std::string scenario_path, int c, char** v);
	GlobalXmlVisitor(std::string file_path, int c, char** v);
	GlobalXmlVisitor(void);
	~GlobalXmlVisitor(void);
};

