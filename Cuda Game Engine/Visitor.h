#pragma once
#define VISITOR_H
#include "Visitor Header.h"
#include "XmlParser.h"
#include "TestEngine.h"


class Visitor
{
protected:
    Shader* loadShaderFromXml(pugi::xml_node& xml_shader_data);
    Texture* loadTextureFromXml(pugi::xml_node& xml_tex_data);
    TriangleMesh loadMeshFromXml(pugi::xml_node& xml_tex_data);
	bool read_data;
	virtual void readData();
	pugi::xml_document doc;
	static ResourceHandler scene_objects;
	std::string resource_path;
    //Issues a call to glTexParameter* based on xml data
    void setGLCodeFromXml(pugi::xml_node xml_input, Texture* texture);
	//Creates a UniformBlock object from an xml node
	UniformBlock synthesizeBlock(pugi::xml_node xml_data);
	//Parses a single uniform for input to a shader
	ShaderParameter parseUniform(pugi::xml_node& xml_data);

public:
	friend class CompositeGrid;
	virtual void visitCompositeGrid(CompositeGrid* tiles);
	Visitor(void);
	~Visitor(void);
};

