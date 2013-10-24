#include "ShaderParameter.h"

ShaderParameter::ShaderParameter(std::string name, int size, int card, float* data, bool to_delete) :
	size(size),
	name(name),
	cardinality(card),
	client_data(data),
	delete_on_buffer(to_delete),
	buffer_handle(0)
{
}


ShaderParameter::~ShaderParameter(void)
{
}

ShaderParameter::ShaderParameter(const ShaderParameter& copy) :
	cardinality(copy.cardinality),
	size(copy.size),
	name(copy.name),
	client_data(copy.client_data),
	shader_handle(copy.shader_handle),
	is_uniform(copy.is_uniform),
	delete_on_buffer(copy.delete_on_buffer),
	buffer_handle(copy.buffer_handle)
{
}

//TODO: Implement this
ShaderParameter& ShaderParameter::operator=(const ShaderParameter& rhs)
{
	cardinality = rhs.cardinality;
	size = rhs.size;
	name = rhs.name;
    delete_on_buffer = rhs.delete_on_buffer;
	
	return *this;
}