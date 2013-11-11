#pragma once
#include <string>
#include <vector>
#include "ShaderParameter.h"
#include "WorldState.h"
#include "Shader.h"

class UniformBlock
{
private:
	std::string block_name;
	void initBuffers();
	bool initialized;
	int buffer_handle;
	int block_index;
	std::vector<ShaderParameter> uniforms;
    int uniform_binding;

public:

    int getBinding();
    //Sets the UBO handle externally for this block.
    void setBufferHandle(int handle);
    std::string getName();
	std::vector<ShaderParameter>& getParameters();
	void addParameter(ShaderParameter to_add);
	UniformBlock(std::string uniform_name, int block_binding);
	UniformBlock(const UniformBlock& copy);
	UniformBlock& operator=(const UniformBlock& rhs);
	~UniformBlock(void);
	void bind();
	void unbind();
};

