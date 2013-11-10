#pragma once
#include "vec3.hpp"
#include "WorldState.h"

class Camera
{
private:
	//View frustum information
	float left;
	float right;
	float bottom;
	float top;
	float near_plane;
	float far_plane;
	vec3 location;
	vec3 view_direction;
	vec3 up_direction;
	GLuint uniform_buffer_handle;
	//Indicates whether or not parameters have changed requiring us
	//to re-update the buffer.
	bool dirty;

	//describes the size of the entire uniform block used by the camera
	static const int buffer_size;

	//Indicates whether or not we have created a buffer for all parameters
	bool initialized;
	void initialize();
	//The array that backs the buffered data in VRAM
	float* buffer_data;
	void setClientsideBuffer();
public:
	friend class CompositeGrid;
	friend class RenderTarget;
	//Called on a mouse movement
	void mouseChanged(int dx, int dy);
	void setLocation(vec3 location);
	void setDirection(vec3 direction);
	void setUpDirection(vec3 up_direction);
	//indicates whether or not we need to rebind this camera's data
	bool isDirty() const;
	//Uses all of the current data from this camera to reset uniform buffers
	void bind();
	Camera(void);
	~Camera(void);
};

