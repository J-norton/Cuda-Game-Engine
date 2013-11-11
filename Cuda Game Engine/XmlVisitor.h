#pragma once
#include "Visitor.h"
#include "ResourceHandler.h"

class XmlVisitor :
	public Visitor
{
private:
	int argc;
	char** argv;
	TestEngine file_handler;
public:
	virtual void visitCompositeGrid(CompositeGrid* tiles);
	XmlVisitor(void);
	XmlVisitor(const XmlVisitor& copy);
	XmlVisitor(std::string file_path, int c, char** v);
	~XmlVisitor(void);
};

