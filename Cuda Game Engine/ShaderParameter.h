#pragma once
#include <string>

class ShaderParameter
{
public:
	//Number of scalars per parameter. Must be between 1 and 4
	int cardinality;
	//Total size of the buffer in bytes
	int size;
	//The name of the parameter in the shader
	std::string name;
	float* client_data;

	int buffer_handle;
	int shader_handle;
	bool is_uniform;

	//Copy Constructor
	ShaderParameter( const ShaderParameter& to_copy);
	ShaderParameter& operator=(const ShaderParameter& rhs);
	bool delete_on_buffer;


	ShaderParameter(std::string, int size, int cardinality, float* data, bool delete_on_buffer);
	ShaderParameter(void);
	~ShaderParameter(void);
};
