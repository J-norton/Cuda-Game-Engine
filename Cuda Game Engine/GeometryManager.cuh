#pragma once
#include <vector>
#include "TriangleMesh.h"
#include "CudaGLBuffer.cuh"
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <helper_cuda.h>
#include <helper_cuda_gl.h>

typedef struct
{
	cudaGraphicsResource_t buffer;
	size_t num_bytes;
	void* dev_arg;
} CudaArg;

class GeometryManager
{
private:
	void launchKernel();
	void runCuda();
	int timestep;

	bool initialized;

	//Temporary argument variables
	void* dev_arg1;
	void* dev_indices1;
	void* dev_arg2;
	void* dev_indices2;

	std::vector<CudaArg> cuda_buffers; 
public:
	virtual void processGeometry(std::vector<TriangleMesh>& meshes);
	GeometryManager();
	~GeometryManager();
};