#include "GeometryManager.cuh"

__device__ float3 operator+(const float3 &a, const float3 &b)
{
	return make_float3(a.x+b.x, a.y+b.y, a.z+b.z);
}

__device__ float3 operator* (const float3 &a, const float &b)
{
	return make_float3(a.x * b, a.y * b, a.z * b);
}


__global__ void simple_vbo_kernel(float *pos1, int num_triangles, int timestep)
{
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	if (x > num_triangles)
	{
		x = 2;
		return;
	}
	int stride = 8;

	float3 v1;
	float3 v2;
	float3 v3;

	int idx_1 = 3 * x * stride;
	int idx_2 = (3 * x + 1) * stride;
	int idx_3 = (3 * x + 2) * stride;

	v1.x = pos1[idx_1];
	v1.y = pos1[idx_1 + 1];
	v1.z = pos1[idx_1 + 2];

	v2.x = pos1[idx_2];
	v2.y = pos1[idx_2 + 1];
	v2.z = pos1[idx_2 + 2];

	v3.x = pos1[idx_3];
	v3.y = pos1[idx_3 + 1];
	v3.z = pos1[idx_3 + 2];

	float small_num = 0.0003;
	float3 middle = v1 + v2 + v3;
	v1 = v1 + middle * small_num;
	v2 = v2 + middle * small_num;
	v3 = v3 + middle * small_num;

	pos1[idx_1] = v1.x;
	pos1[idx_1 + 1] = v1.y;
	pos1[idx_1 + 2] = v1.z;

	pos1[idx_2] = v2.x;
	pos1[idx_2 + 1] = v2.y;
	pos1[idx_2 + 2] = v2.z;

	pos1[idx_3] = v3.x;
	pos1[idx_3 + 1] = v3.y;
	pos1[idx_3 + 2] = v3.z;
}

GeometryManager::GeometryManager() :
	initialized(false),
	timestep(0)
{
}

void GeometryManager::processGeometry(std::vector<TriangleMesh>& meshes)
{
	vec3 posn(0.0, 0.0, -5.0);

	if (!initialized)
	{
		if (cuda_buffers.size() < meshes.size())
		{
			for (TriangleMesh& mesh : meshes)
			{
				if (mesh.static_object)
					continue;
				//CudaArg to_add = {mesh.getState().getBufferManager().getVBO(), 0};
				//cuda_buffers.push_back( to_add );
			}
		}
		initialized = true;
	}
	runCuda();
}

void GeometryManager::runCuda()
{
	cudaError_t error_code = cudaSuccess;
	size_t num_bytes;
	size_t num_bytes_ibo;

	for (CudaArg& arg : cuda_buffers)
	{
		error_code = cudaGraphicsMapResources(1, &arg.buffer, 0);
		if (error_code != cudaSuccess)
			std::cout << "Cuda error mapping GL buffer" << std::endl;

		error_code = cudaGraphicsResourceGetMappedPointer(&arg.dev_arg, &arg.num_bytes, arg.buffer);
		if (error_code != cudaSuccess)
			std::cout << "Cuda error mapping GL buffer" << std::endl;
	}

	int timestep;
	launchKernel();

	for (int i = 0; i < cuda_buffers.size(); i++)
	{
		error_code = cudaGraphicsUnmapResources(1, &cuda_buffers[i].buffer, 0);
		if (error_code != cudaSuccess)
			std::cout << "Cuda error unmapping GL buffer" << std::endl;
	}
}

void GeometryManager::launchKernel()
{
	for (CudaArg& arg : cuda_buffers)
	{
		int num_triangles = arg.num_bytes / 96;
		dim3 block(num_triangles, 1, 1);
		dim3 threads_per_block(1, 1, 1);
		simple_vbo_kernel<<<block, threads_per_block>>>((float*)arg.dev_arg, num_triangles, timestep++);
	}
}


GeometryManager::~GeometryManager()
{
}