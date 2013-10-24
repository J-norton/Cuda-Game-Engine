#include "UniformBlock.h"


UniformBlock::UniformBlock(std::string uniform_name, int block_binding) :
	initialized(false),
	buffer_handle(0),
    uniform_binding(block_binding)
{
	block_name = uniform_name;
}


UniformBlock::~UniformBlock(void) 
{
}

void UniformBlock::bind()
{
	//if (!initialized)
	//{
	//	initBuffers();
	//	initialized = true;
	//}

	glBindBuffer(GL_UNIFORM_BUFFER, buffer_handle);
}

void UniformBlock::unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::string UniformBlock::getName()
{
    return block_name;
}

void UniformBlock::initBuffers()
{
	block_index = WorldState::getCurrentShader()->getUniformBlockIndex(block_name);
	if (block_index == -1)
	{
		std::cout << "There is no uniform block named: " << block_name << " in the" << std::endl
			<< " current shader." << std::endl;
		return;
	}

	//Query for the size of the uniform block
	int block_size = WorldState::getCurrentShader()->getBlockSize(block_name);

	GLuint* handle = new GLuint[1];
	int uniform_size = 0;

	for (ShaderParameter param : uniforms)
		uniform_size += param.size;

	float* data = new float[block_size / 4];
	int k = 0;
	//First initialize each uniform in the block
	for (int i = 0; i < uniforms.size(); i++)
	{
		ShaderParameter param = uniforms[i];
		int prog_id = WorldState::getCurrentShader()->getProgHandle();
		unsigned int index = WorldState::getCurrentShader()->getUniformIndex(param.name);
		int size;
		int offset;
		int type;
		glGetActiveUniformsiv(prog_id, 1, &index, GL_UNIFORM_OFFSET, &offset);
		glGetActiveUniformsiv(prog_id, 1, &index, GL_UNIFORM_SIZE, &size);
		glGetActiveUniformsiv(prog_id, 1, &index, GL_UNIFORM_TYPE, &type);
		for (int i = 0; i < param.size / 4; i++)
			data[offset / 4 + i] = param.client_data[i];
	}

	//Now buffer the data
	glGenBuffers(1, handle);
	buffer_handle = *handle;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer_handle);
	glBufferData(GL_UNIFORM_BUFFER, block_size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, block_index, buffer_handle);

	//Unbind uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	delete[] data;
	//TODO: Clean up memory leaks
	for (ShaderParameter param : uniforms)
		if (param.delete_on_buffer)
		{
			delete[] param.client_data;
			param.delete_on_buffer = false;
		}
}

void UniformBlock::setBufferHandle(int handle)
{
    buffer_handle = handle;
}

void UniformBlock::addParameter(ShaderParameter to_add)
{
	uniforms.push_back(to_add);
}

UniformBlock::UniformBlock(const UniformBlock& copy) :
	block_index(copy.block_index),
	block_name(copy.block_name),
	buffer_handle(copy.buffer_handle),
	initialized(copy.initialized),
	uniforms(copy.uniforms),
    uniform_binding(copy.uniform_binding)
{
	//for (int i = 0; i < copy.uniforms.size(); i++)
	//	uniforms.push_back(copy.uniforms[i]);
}

UniformBlock& UniformBlock::operator=(const UniformBlock& rhs)
{
    uniform_binding = rhs.uniform_binding;
	block_index = rhs.block_index;
	block_name = rhs.block_name;
	buffer_handle = rhs.buffer_handle;
	initialized = rhs.initialized;
	uniforms = rhs.uniforms;

	return *this;
}


std::vector<ShaderParameter>& UniformBlock::getParameters()
{
    return uniforms;
}

int UniformBlock::getBinding()
{
    return uniform_binding;
}