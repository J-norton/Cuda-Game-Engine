#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "WorldState.h"
#include "ScenarioException.h"
#include <vector>
#include "UniformBlock.h"
#include "vec3.hpp"

class Shader
{
private:
	//Indicates whether or not we need to resend uniforms to the GL when binding
	bool dirty_uniforms;
    //GL Handles to program object and shaders.
	int program_handle;
	int vs_handle;
	int fs_handle;
	std::string vs_shader_path;
	std::string fs_shader_path;
    //Tracks whether the shader has been compiled
	bool compiled;
    //Tracks whether the UniformBlocks have been buffered
    bool uniform_buffered;
	void compileShaders();
    void validate();
    //All uniform block instances that are attached to this shader.
    std::vector<UniformBlock> uniforms;
    //All uniform inputs that are attached to this shader.
    std::vector<ShaderParameter> uniform_inputs;

    //All uniform block instances that are attached to this shader.
    std::vector<std::string> transform_outs;

    GLuint uniform_buffer_handle;

	//Takes all the current uniform inputs to the shader and rebinds them
	void bindUniforms();
	//Initializes all uniform block buffers associated with the shader in one pass
    void initializeUniformBlocks();

public:
	//Sets a uniform, must be a vec3
	void setUniform(std::string name, vec3 new_value);
	//Adds a uniform input to the shader
	void addUniform(ShaderParameter uniform);
    void addTransformedOutput(std::string output_name);
    void addUniformBlock(UniformBlock block);
	int getProgHandle();
	int getBlockSize(std::string block_name);
	unsigned int getUniformIndex(std::string name);
	unsigned int getUniformBlockIndex(std::string name);
	//Returns IDs associated with the shader input param_name
	int getUniform(std::string param_name);
	int getAttrib(std::string param_name);
	void bind();
	void unbind();
	Shader(std::string vs_shader_path, std::string fs_shader_path);
	Shader();
	Shader(const Shader& copy);
	Shader& operator= (const Shader& rhs);
	~Shader(void);
};
