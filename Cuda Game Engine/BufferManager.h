#pragma once
#include <iostream>
#include <vector>
#include "ShaderParameter.h"
#include "WorldState.h"
#include "Shader.h"

class BufferManager
{
protected:
	int interleave_handle;
	int index_handle;
	bool has_indices;
	int num_vertices;
	float* data;
	bool bound;
	std::vector<ShaderParameter> parameters;
	virtual void initBuffers();
	void bindIndices();
	int stride;

public:
	virtual void initIndexBuffer(int* data, int size);
	int getNumVertices();
	virtual std::string getType();
	virtual void bind();
	virtual void unbind();
	void addBuffer(ShaderParameter param);
	BufferManager(void);
	~BufferManager(void);
};
