#pragma once
#include "ObjectState.h"


class TriangleMesh
{
protected:
	ObjectState mesh_state;
	int num_instances;
    float counter;

public:
    void setTexture(Texture new_texture);
	void setText(std::string message);
	void draw();
	TriangleMesh(ObjectState draw_state);
	TriangleMesh(ObjectState draw_state, int num_instances);
	TriangleMesh(void);
	TriangleMesh(const TriangleMesh& copy);
	TriangleMesh& operator=(const TriangleMesh& rhs);
	~TriangleMesh(void);
};

