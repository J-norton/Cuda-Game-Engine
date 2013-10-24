#include "Shader.h"


Shader::Shader(std::string vs_path, std::string fs_path) :
	compiled(false),
	uniforms(std::vector<UniformBlock>()),
	uniform_inputs(std::vector<ShaderParameter>()),
	uniform_buffer_handle(0),
	uniform_buffered(false)
{
	std::string dir = "../Test Files/Shader Programs/";
	vs_shader_path = dir.append(vs_path);
	dir = "../Test Files/Shader Programs/";
	fs_shader_path = dir.append(fs_path);
}

Shader::Shader() :
	compiled(false),
	uniforms(std::vector<UniformBlock>())
{
}


Shader::~Shader(void)
{
}

int Shader::getAttrib(std::string param_name)
{
	validate();
	if(!compiled)
		compileShaders();
	return glGetAttribLocation(program_handle, param_name.c_str());
}

void Shader::compileShaders()
{
	GLint frag_compiled, compile_worked, linked;

	std::ifstream src_file = std::ifstream(vs_shader_path);
	std::string src_prog(std::istreambuf_iterator<char>(src_file), (std::istreambuf_iterator<char>()) );
	const char* cstr_source = src_prog.c_str();

	std::ifstream fragsrc_file = std::ifstream(fs_shader_path);
	std::string frag_prog(std::istreambuf_iterator<char>(fragsrc_file), (std::istreambuf_iterator<char>()) );
	const char* cstr_frag = frag_prog.c_str();

	vs_handle = glCreateShader(GL_VERTEX_SHADER);
	fs_handle = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs_handle, 1, &cstr_source, nullptr);
	glShaderSource(fs_handle, 1, &cstr_frag, nullptr);
	glCompileShader(vs_handle);
	glCompileShader(fs_handle);
	glGetShaderiv(vs_handle, GL_COMPILE_STATUS, &compile_worked);
	glGetShaderiv(fs_handle, GL_COMPILE_STATUS, &frag_compiled);

	if (compile_worked == 0)
	{
		//std::cout << "Error Compiling GLSL code" << std::endl;
		GLint length;
		GLchar* error_log;
		glGetShaderiv(vs_handle, GL_INFO_LOG_LENGTH, &length);

		error_log = new GLchar[length];
		glGetShaderInfoLog(vs_handle, length, &length, error_log);
		std::cout << error_log;
		std::string arg = std::string(error_log);
		delete[] error_log;
		throw ScenarioException(arg);
	}

	if (frag_compiled == 0)
	{
		//std::cout << "Fragment shader did not compile correctly" << std::endl;
		GLint length;
		GLchar* error_log;
		glGetShaderiv(fs_handle, GL_INFO_LOG_LENGTH, &length);

		error_log = new GLchar[length];
		glGetShaderInfoLog(fs_handle, length, &length, error_log);
		std::cout << error_log;
		std::string arg = std::string(error_log);
		delete[] error_log;
		throw ScenarioException(arg);
	}
	program_handle = glCreateProgram();

	glAttachShader(program_handle, vs_handle);
	glAttachShader(program_handle, fs_handle);
	glLinkProgram(program_handle);

	GLint uniform_count;
	int code = glGetError();
	glGetProgramiv(program_handle, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_count);
	code = glGetError();

	glGetProgramiv(program_handle, GL_LINK_STATUS, &linked);
	code = glGetError();

	if (linked)
	{
		glUseProgram(program_handle);
	}
	else
	{
		//std::cout << "Error linking GLSL Program" << std::endl;
		GLint length;
		GLchar* error_log;
		glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &length);
		if (length < 1)
			length = 1024;
		error_log = new GLchar[length];

		//glGetShaderInfoLog(fs_handle, length, &length, error_log);
		glGetProgramInfoLog(program_handle, length, &length, error_log);
		std::cout << error_log;
		std::string arg = std::string(error_log);
		delete[] error_log;
		throw ScenarioException(arg);
	}
	compiled = true;
}

void Shader::addUniformBlock(UniformBlock block)
{
	uniforms.push_back(block);
}

//Makes this shader the currently used shader for rendering
void Shader::bind()
{
	if (!compiled)
		compileShaders();

	glUseProgram(program_handle);

	bindUniforms();

	if (!uniform_buffered && uniforms.size() > 0)
	{
		initializeUniformBlocks();
		uniform_buffered = true;
	}
}

void Shader::addUniform(ShaderParameter to_add)
{
	uniform_inputs.push_back(to_add);
}

void Shader::bindUniforms()
{
	for (ShaderParameter& param : uniform_inputs)
	{
		switch(param.cardinality)
		{
		case 1:
			glUniform1f(getUniform(param.name), param.client_data[0]);
			break;
		case 2:
			glUniform2f(getUniform(param.name), param.client_data[0], param.client_data[1]);
			break;
		case 3:
			glUniform3f(getUniform(param.name), param.client_data[0], param.client_data[1], param.client_data[2]);
			break;
		case 4:
			glUniform4f(getUniform(param.name), param.client_data[0], param.client_data[1], param.client_data[2], param.client_data[3]);
			break;
		}
		if (glGetError() != GL_NO_ERROR)
			std::cout << "Error occurred binding uniform input" << std::endl;
	}
	for (ShaderParameter& p : uniform_inputs)
	{
		if (strcmp(p.name.c_str(), "position") == 0)
			p.client_data[0] += 0.3;
	}
}

void Shader::unbind()
{
	glUseProgram(0);
}

unsigned int Shader::getUniformBlockIndex(std::string uniform_name)
{
	validate();
	const char* pass = uniform_name.c_str();
	int err = glGetError();
	if (glGetUniformBlockIndex == NULL)
		std::cout << "Get UBO Index Worked" << std::endl;
	int to_return = glGetUniformBlockIndex(program_handle, pass );
	return to_return;
}

unsigned int Shader::getUniformIndex(std::string name)
{
	validate();
	GLuint to_return;
	const char* check = name.c_str();
	glGetUniformIndices(program_handle, 1, (const GLchar**) &check, &to_return);

	if (to_return == GL_INVALID_INDEX)
	{
		std::cout << "The variable " << name << " is not associated with the"
			<< " current shader.\n";
	}
	return to_return;
}

int Shader::getBlockSize(std::string block_name)
{
	validate();
	unsigned int block_id = getUniformBlockIndex(block_name);
	int to_return;
	//If the parameter doesn't belong to the shader or is optimized out, return -1
	if (block_id == GL_INVALID_INDEX)
		return -1;
	glGetActiveUniformBlockiv(program_handle, block_id, GL_UNIFORM_BLOCK_DATA_SIZE, &to_return);
	return to_return;
}

int Shader::getProgHandle()
{
	validate();
	return program_handle;
}

int Shader::getUniform(std::string param_name)
{
	validate();
	int to_return = glGetUniformLocation(program_handle, param_name.c_str());
	return to_return;
}

void Shader::validate()
{
	GLint shdr_check;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shdr_check);
	if (shdr_check != program_handle)
		std::cout << "Shader bound incorrectly" << std::endl;
}

void Shader::addTransformedOutput(std::string output_name)
{
	transform_outs.push_back(output_name);
}


void Shader::initializeUniformBlocks()
{
	GLint align_length;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align_length);
	GLint ubuffer_count;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &ubuffer_count);
	//The total size of every uniform block in bytes
	int block_buffer_size = 0;
	for (int i = 0; i < uniforms.size(); i++)
	{
		UniformBlock block = uniforms[i];
		std::string block_name = block.getName();
		//int bsize = WorldState::getCurrentShader()->getBlockSize(block_name);
		int bsize = getBlockSize(block_name);
		if (glGetError() != GL_NO_ERROR)
			std::cout << "Uniform Block buffer error" << std::endl;
		if (bsize < 0)
		{
			uniforms.erase(uniforms.begin() + i);
			continue;
		}
		//Compute the number of alignment lengths needed by the buffer
		int num_aligns = bsize / align_length + 1;
		block_buffer_size += num_aligns * align_length;
	}

	//data will contain all of the data needed for every UniformBlock
	float* data = new float[block_buffer_size / 4];

	for (int i = 0; i < block_buffer_size / 4; i++)
		data[i] = 0;
	//Generate the buffer object that will supply all of the data for all uniform blocks
	glGenBuffers(1, &uniform_buffer_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_handle);
	//Allocate 'block_buffer_size' bytes of data on the GPU
	glBufferData(GL_UNIFORM_BUFFER, block_buffer_size, nullptr, GL_STATIC_DRAW);

	//stores where in the buffer should put data for the current UniformBlock 
	int buffer_location = 0;
	for (UniformBlock block : uniforms)
	{
		std::string block_name = block.getName();
		std::vector<ShaderParameter> uniform_vars = block.getParameters();
		int block_index = getUniformBlockIndex(block_name);
		if (block_index == -1)
		{
			std::cout << "There is no uniform block named: " << block_name << " in the" << std::endl
				<< " currently bound shader." << std::endl;
			return;
		}
		int prog_id = getProgHandle();
		GLint binding_pt;
		glGetActiveUniformBlockiv(prog_id, block_index, GL_UNIFORM_BLOCK_BINDING, &binding_pt);
		if (glGetError() != GL_NO_ERROR)
			std::cout << "Uniform Block buffer error" << std::endl;

		//Query for the size of the uniform block
		//int block_size = WorldState::getCurrentShader()->getBlockSize(block_name);
		int block_size = getBlockSize(block_name);
		int num_aligns = block_size / align_length + 1;
		int aligned_buffer_size = num_aligns * align_length;

		//First initialize each uniform in the block
		for (int i = 0; i < uniform_vars.size(); i++)
		{
			ShaderParameter param = uniform_vars[i];
			//unsigned int index = WorldState::getCurrentShader()->getUniformIndex(param.name);
			unsigned int index = getUniformIndex(param.name);
			int size;
			int offset;
			int type;
			glGetActiveUniformsiv(prog_id, 1, &index, GL_UNIFORM_OFFSET, &offset);
			glGetActiveUniformsiv(prog_id, 1, &index, GL_UNIFORM_SIZE, &size);
			glGetActiveUniformsiv(prog_id, 1, &index, GL_UNIFORM_TYPE, &type);
			for (int j = 0; j < param.size / 4; j++)
			{
				int storage_point = offset / 4 + buffer_location / 4 + j;
				data[storage_point] = param.client_data[j];
			}
		}

		//Now buffer the data
		glBufferSubData(GL_UNIFORM_BUFFER, buffer_location, block_size, data + (buffer_location / 4));
		if (glGetError() != GL_NO_ERROR)
			std::cout << "Uniform Block buffer error" << std::endl;
		glBindBufferRange(GL_UNIFORM_BUFFER, block.getBinding(), uniform_buffer_handle, buffer_location, block_size);
		GLenum errc = glGetError();
		if (errc != GL_NO_ERROR)
			std::cout << errc << std::endl << "Uniform Block buffer error" << std::endl;

		for (ShaderParameter param : uniform_vars)
		{
			if (param.delete_on_buffer)
			{
				delete[] param.client_data;
				param.delete_on_buffer = false;
			}
		}

		buffer_location += aligned_buffer_size;
	}

	delete[] data;
	//Unbind uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}