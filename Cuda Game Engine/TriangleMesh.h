#pragma once
#include "Shader.h"
#include "BufferManager.h"
#include "UniformBlock.h"
#include "Texture.h"
#include "WorldState.h"
//#include "ObjectState.h"
#include "vec3.hpp"

class TriangleMesh
{
protected:
	Shader shader;
	BufferManager buffers;
	Texture texture;
	//ObjectState mesh_state;
	int num_instances;
    float counter;
	vec3 velocity;
	vec3 position;
	vec3 scale;

	bool static_object;

public:
	friend class GeometryManager;
	//Resets the values of all uniforms to this meshes shader
	void updateUniforms();
	void setVelocity(vec3 new_value);
	void setPosition(vec3 new_value);
	void setScale(vec3 new_value);
    void setTexture(Texture new_texture);
	void draw();

	ObjectState& getState();

	//TriangleMesh(bool static_mesh = false);
	TriangleMesh(Shader shader, BufferManager buffer, Texture texture, bool static_mesh = false);
	TriangleMesh(int num_instances);
	TriangleMesh(void);
	TriangleMesh(const TriangleMesh& copy);
	TriangleMesh& operator=(const TriangleMesh& rhs);
	~TriangleMesh(void);
};

