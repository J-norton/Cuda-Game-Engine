
#include "CudaGLBuffer.cuh"


__device__ float3 operator+(const float3 &a, const float3 &b)
{
  return make_float3(a.x+b.x, a.y+b.y, a.z+b.z);
}

__device__ float3 operator* (const float3 &a, const float &b)
{
	return make_float3(a.x * b, a.y * b, a.z * b);
}

__global__ void simple_vbo_kernel(float *pos, int *indices, int num_vertices, int timestep)
{
	unsigned int x = blockIdx.x * 3;
	//unsigned int giu = blockDim.x;
	//unsigned int dij = threadIdx.y;
	int stride = 8;

	float3 v1;
	float3 v2;
	float3 v3;

	int idx_1 = indices[x] * stride;
	int idx_2 = indices[x + 1] * stride;
	int idx_3 = indices[x + 2] * stride;

	v1.x = pos[idx_1];
	v1.y = pos[idx_1 + 1];
	v1.z = pos[idx_1 + 2];

	v2.x = pos[idx_2];
	v2.y = pos[idx_2 + 1];
	v2.z = pos[idx_2 + 2];

	v3.x = pos[idx_3];
	v3.y = pos[idx_3 + 1];
	v3.z = pos[idx_3 + 2];

	float small_num = 0.001;
	float3 middle = v1 + v2 + v3;
	v1 = v1 + middle * small_num;
	v2 = v2 + middle * small_num;
	v3 = v3 + middle * small_num;

	pos[idx_1] = v1.x;
	pos[idx_1 + 1] = v1.y;
	pos[idx_1 + 2] = v1.z;

	pos[idx_2] = v2.x;
	pos[idx_2 + 1] = v2.y;
	pos[idx_2 + 2] = v2.z;

	pos[idx_3] = v3.x;
	pos[idx_3 + 1] = v3.y;
	pos[idx_3 + 2] = v3.z;

	//pos[idx_1] = pos[idx_1] + sinf(timestep / 9) / 15.0;
	//pos[idx_1 + 1] = pos[idx_1 + 1] + cosf(timestep / 9) / 15.0;
	//pos[idx_1 + 2] = pos[idx_1 + 2] + cosf(timestep / 9) / 15.0;







	//float mul = 0.9;
	//for (int i = 0; i < 3; i++)
	//{
	//	pos[idx_1] = pos[idx_1] * mul;
	//	pos[idx_2] = pos[idx_2] * mul;
	//	pos[idx_3] = pos[idx_3] * mul;
	//	idx_1 += 1;
	//	idx_2 += 1;
	//	idx_3 += 1;
	//}
}

CudaGLBufferObj::CudaGLBufferObj(void)
{
}

CudaGLBufferObj::~CudaGLBufferObj(void)
{
}

std::string CudaGLBufferObj::getType()
{
	return "CUDA + GL VBO";
}

void CudaGLBufferObj::bind()
{
	//If the parameter size is zero, there is no data to bind to the GL
	if (parameters.size() == 0) {
		std::cout << "Empty buffers for shader object: \n";
		return;
	}

	if (!bound) {
		initBuffers();
		bound = true;
	}
	else
		runCuda();

	glBindVertexArray(vao_handle);
	if (has_indices)
		bindIndices();
}

void CudaGLBufferObj::runCuda()
{
	cudaError_t error_code = cudaSuccess;
	error_code = cudaGraphicsMapResources(1, &cuda_vbo, 0);
	if (error_code != cudaSuccess)
		std::cout << "Cuda error mapping GL buffer" << std::endl;

	size_t num_bytes;
	size_t num_bytes_ibo;
	error_code = cudaGraphicsResourceGetMappedPointer(&dev_arg1, &num_bytes, cuda_vbo);
	if (error_code != cudaSuccess)
		std::cout << "Cuda error mapping GL buffer" << std::endl;

	error_code = cudaGraphicsMapResources(1, &cuda_ibo, 0);
	if (error_code != cudaSuccess)
		std::cout << "Cuda error mapping GL index buffer" << std::endl;

	error_code = cudaGraphicsResourceGetMappedPointer(&dev_indices, &num_bytes_ibo, cuda_ibo);
	if (error_code != cudaSuccess)
		std::cout << "Cuda error mapping GL buffer" << std::endl;

	int timestep;
	launchKernel();
	error_code = cudaGraphicsUnmapResources(1, &cuda_vbo, 0);
	if (error_code != cudaSuccess)
		std::cout << "Cuda error unmapping GL buffer" << std::endl;
	error_code = cudaGraphicsUnmapResources(1, &cuda_ibo, 0);
	if (error_code != cudaSuccess)
		std::cout << "Cuda error unmapping GL index buffer" << std::endl;
}

void CudaGLBufferObj::initIndexBuffer(int* indices, int n)
{
	has_indices = true;
	unsigned int* temp = new unsigned int[1];
	num_vertices = n / 4;

	glGenBuffers(1, temp);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp[0]);
	index_handle = temp[0];
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n, indices,
		GL_STATIC_DRAW);
	// Unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cudaError_t error = cudaGraphicsGLRegisterBuffer(&cuda_ibo, index_handle, cudaGraphicsMapFlagsWriteDiscard);

	if (error != cudaSuccess)
		std::cout << "Error occurred mapping index buffer to CUDA" << std::endl;

	//Now that the memory is buffered, delete the client-side ints
	delete[] indices;
}

void CudaGLBufferObj::launchKernel()
{
	return;
	dim3 block(num_vertices / 3, 1, 1);
	dim3 threads_per_block(1, 1, 1);
	simple_vbo_kernel<<<block, threads_per_block>>>((float*)dev_arg1, (int*) dev_indices, num_vertices, timestep++);
}

void CudaGLBufferObj::unbind()
{
	glBindVertexArray(0);
}

void CudaGLBufferObj::initBuffers()
{
	timestep = 0;
	//First determine the size of the entire buffer
	int buffer_size = 0;
	//Represents the number of bytes between the ith and i + 1th item in the buffer
	stride = 0;
	for (ShaderParameter p : parameters)
	{
		buffer_size += p.size;
		stride += p.cardinality * 4;
		p.shader_handle = WorldState::getCurrentShader()->getAttrib(p.name);
	}

	int vertex_count = buffer_size / stride;
	float* interleaved_data = new float[buffer_size / 4];
	int k = 0;

	//The following assumes that all parameters have the same number of elements
	for (int i = 0; i < vertex_count; i++)
	{
		for (ShaderParameter p : parameters)
		{
			//The index into the array that will
			int offset = p.cardinality * i;
			for (int j = 0; j < p.cardinality; j++)
				interleaved_data[k++] = (p.client_data[offset + j]);
		}
	}
	//Create a Vertex Array Object so that we can bind the VBOs to it
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);
	GLuint* buffer_return = new GLuint[1];
	//Finally generate and bind the buffer
	glGenBuffers(1, buffer_return);
	interleave_handle = buffer_return[0];
	glBindBuffer(GL_ARRAY_BUFFER, interleave_handle);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, &interleaved_data[0], GL_STATIC_DRAW);
	//Describe the layout of the data to the GL
	GLvoid* offset = 0;
	int address = 0;
	for (ShaderParameter parameter : parameters)
	{
		//GLuint shader_num = WorldState::current_shader->getAttrib(parameter.name);
		GLuint shader_num = WorldState::getCurrentShader()->getAttrib(parameter.name);
		if (shader_num == -1)
		{
			address += parameter.cardinality * 4;
			offset = (GLvoid*) address;
			continue;
		}
		glVertexAttribPointer(shader_num, parameter.cardinality, GL_FLOAT, false, stride, offset);
		if (glGetError() != 0)
			std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";

		address += parameter.cardinality * 4;
		offset = (GLvoid*) address;
		glEnableVertexAttribArray(shader_num);
		if (glGetError() != 0)
			std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";
	}
	//Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	cudaError_t err = cudaGraphicsGLRegisterBuffer(&cuda_vbo, interleave_handle, cudaGraphicsMapFlagsWriteDiscard);
	if (err != 0)
		std::cout << "Could not map GL Buffer to CUDA" << std::endl;

	delete[] buffer_return;
	delete[] interleaved_data;
	//Delete all individual client-side arrays
	for (ShaderParameter p : parameters)
	{
		if (p.delete_on_buffer)
			delete[] p.client_data;
	}
	bound = true;
}

cudaGraphicsResource_t CudaGLBufferObj::getVBO()
{
	return cuda_vbo;
}

cudaGraphicsResource_t CudaGLBufferObj::getIBO()
{
	return cuda_ibo;
}