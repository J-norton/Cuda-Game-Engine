#include "Node.h"


Node::Node(void)
{
}


Node::~Node(void)
{
}

void Node::acceptVisitor(Visitor* v)
{
	v->visitNode(this);
}

void Node::draw()
{
	for (TriangleMesh& tmesh : meshes)
		tmesh.draw();

	for (Node* child : children)
		child->draw();
}