#include "Light.h"

//Location is 3 floats, intensity is 1 float, color is 3 floats
const int Light::buffer_size = 7 * sizeof(float);

Light::Light(void):
	dirty(true),
	location(vec3(0, 0, 1)),
	color(vec3(0, 0, 0)),
	intensity(0.0),
	initialized(false)
{
}


Light::~Light(void)
{
}

void Light::setLocation(vec3 location)
{
	this->location = location;
}

void Light::setColor(vec3 color)
{
	this->color = color;
}

void Light::setIntensity(float intensity)
{
	this->intensity = intensity;
}

bool Light::isDirty() const
{
	return dirty;
}

void Light::bind()
{
	if (!initialized)
		initialize();
	else
	{
		setClientsideBuffer();
		glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_handle);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer_size, buffer_data.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		dirty = false;
	}
}

void Light::setClientsideBuffer()
{
	if (buffer_data.size() * sizeof(float) < buffer_size)
	{
		buffer_data.resize(buffer_size * sizeof(float));
	}
	//This describes the layout of of memory within the uniform block buffer
	//All of the data will be accessed through vec4s
	buffer_data[0] = location.x;
	buffer_data[1] = location.y;
	buffer_data[2] = location.z;
	buffer_data[3] = intensity;

	buffer_data[4] = color.x;
	buffer_data[5] = color.y;
	buffer_data[6] = color.z;
}

void Light::initialize()
{
	GLint align_length;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align_length);
	GLint ubuffer_count;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &ubuffer_count);
	//The total size of every uniform block in bytes
	int block_buffer_size = sizeof(float) * 3 * 3;
	int num_aligns = block_buffer_size / align_length + 1;
	block_buffer_size = num_aligns * align_length;

	//data will contain all of the data needed 
	//buffer_data = new float[block_buffer_size / 4];

	for (int i = 0; i < block_buffer_size / 4; i++)
		buffer_data.push_back(0);
	//Generate the buffer object 
	glGenBuffers(1, &uniform_buffer_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_handle);
	//Allocate 'block_buffer_size' bytes of data on the GPU
	glBufferData(GL_UNIFORM_BUFFER, block_buffer_size, nullptr, GL_STATIC_DRAW);

	//Attaches all of the data in buffer_data
	setClientsideBuffer();

	//Now buffer the data
	glBufferSubData(GL_UNIFORM_BUFFER, 0, block_buffer_size, buffer_data.data());
	if (glGetError() != GL_NO_ERROR)
		std::cout << "Uniform Block buffer error" << std::endl;
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uniform_buffer_handle, 0, block_buffer_size);
	GLenum errc = glGetError();
	if (errc != GL_NO_ERROR)
		std::cout << errc << std::endl << "Uniform Block buffer error" << std::endl;

	//Unbind uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	dirty = false;
	initialized = true;
}