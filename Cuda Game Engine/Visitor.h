#pragma once
#define VISITOR_H
#include "Node Visitor Header.h"
#include <pugixml.hpp>
#include "ResourceHandler.h"

class Node;

class Visitor
{
protected:
    Shader* loadShaderFromXml(pugi::xml_node& xml_shader_data);
	BufferManager* parseTransformFeedback(pugi::xml_node& xml_data, Shader& shader);
    Texture loadTextureFromXml(pugi::xml_node& xml_tex_data);
    TriangleMesh loadMeshFromXml(pugi::xml_node& xml_tex_data, Node& container);
	bool read_data;
	virtual void readData();
	pugi::xml_document doc;
	static ResourceHandler scene_objects;
	std::string resource_path;
    //Issues a call to glTexParameter* based on xml data
    void setGLCodeFromXml(pugi::xml_node xml_input, Texture& texture);
	//Creates a UniformBlock object from an xml node
	UniformBlock synthesizeBlock(pugi::xml_node xml_data);
	//Parses a single uniform for input to a shader
	ShaderParameter parseUniform(pugi::xml_node& xml_data);

	//Reads in the camera data in the xml node and sets it accordingly
	//void parseCamera(pugi::xml_node& xml_data, CompositeGrid* target);

public:
	virtual void visitNode(Node* tiles);
	Visitor(void);
	~Visitor(void);
};

