
#pragma once
#include "BufferManager.h"
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <helper_cuda.h>
#include <helper_cuda_gl.h>

class CudaGLBufferObj :
	public BufferManager
{
protected:
	unsigned int vao_handle;
	cudaGraphicsResource_t cuda_vbo;
	cudaGraphicsResource_t cuda_ibo;
	void runCuda();
	void* dev_arg1;
	void* dev_indices;
	void launchKernel();
	int timestep;
public:
	virtual void initIndexBuffer(int* data, int size);
	virtual std::string getType();
	virtual void bind();
	virtual void unbind();
	virtual void initBuffers();
	CudaGLBufferObj(void);
	~CudaGLBufferObj(void);
};

struct dumb
{
	int d;
};
struct dumb2
{
	int e;
};

