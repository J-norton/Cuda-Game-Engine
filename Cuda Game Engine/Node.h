#pragma once
#include <vector>
#include "TriangleMesh.h"
#define NODE_H_HEADER
#include "Node Visitor Header.h"
#include "Light.h"


class Node
{
private:
	std::vector<TriangleMesh> meshes;
	std::vector<Node*> children;
	std::vector<Light> light_sources;
	static const int max_num_light_sources;
	static int current_num_lights;
public:
	friend class Visitor;
	friend class GlobalXmlVisitor;
	void acceptVisitor(Visitor* v);
	void draw();
	Node(void);
	~Node(void);
};

