#pragma once
#include "vec3.hpp"
#include "WorldState.h"
#include <vector>

class Light
{
private:
	vec3 location;

	vec3 color;
	float intensity;
	//Whether or not we need to re-buffer the data
	bool dirty;
	//GL Buffer handle
	GLuint uniform_buffer_handle;
	//How large buffers are for objects of this type
	static const int buffer_size;
	bool initialized;
	void initialize();
	std::vector<float> buffer_data;
	void setClientsideBuffer();

public:
	void setLocation(vec3 location);
	void setColor(vec3 color);
	void setIntensity(float intensity);
	bool isDirty() const;
	void bind();
	Light(void);
	~Light(void);
};

