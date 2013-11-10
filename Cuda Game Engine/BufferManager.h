#pragma once
#include <iostream>
#include <vector>
#include "ShaderParameter.h"
#include "WorldState.h"
#include "Shader.h"
#include <cuda_gl_interop.h>

class BufferManager
{
protected:
	unsigned int vao_handle;
	cudaGraphicsResource_t cuda_vbo;
	cudaGraphicsResource_t cuda_ibo;
	void runCuda();
	void* dev_arg1;
	void* dev_indices;
	void launchKernel();

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
	cudaGraphicsResource_t getVBO();
	cudaGraphicsResource_t getIBO();
	virtual void initIndexBuffer(int* data, int size);
	int getNumVertices();
	virtual std::string getType();
	virtual void bind();
	virtual void unbind();
	void addBuffer(ShaderParameter param);
	BufferManager(void);
	BufferManager& operator= (const BufferManager& rhs);
	BufferManager(const BufferManager& copy);
	~BufferManager(void);
};
