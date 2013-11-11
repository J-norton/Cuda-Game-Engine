#pragma once
#include <vector>
#include "TriangleMesh.h"
#define NODE_H_HEADER
#include "Node Visitor Header.h"


class Node
{
private:
	std::vector<TriangleMesh> meshes;
	std::vector<Node*> children;
public:
	friend class Visitor;
	friend class GlobalXmlVisitor;
	void acceptVisitor(Visitor* v);
	void draw();
	Node(void);
	~Node(void);
};

