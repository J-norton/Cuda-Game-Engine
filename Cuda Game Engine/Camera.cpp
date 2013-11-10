#include "Camera.h"

const int Camera::buffer_size = 68;

Camera::Camera(void) :
	dirty(true),
	view_direction(vec3(0, 0, 1)),
	up_direction(vec3(0, 1, 0)),
	location(vec3(0, 0, 0)),
	near_plane(0.1),
	far_plane(1000.0),
	left(-0.1),
	right(0.1),
	bottom(-0.1),
	top(0.1),
	initialized(false)
{
}


Camera::~Camera(void)
{
}

bool Camera::isDirty() const
{
	return dirty;
}

//Resets the data within the uniform block for the camera
void Camera::bind()
{
	if (!initialized)
		initialize();
	else
	{
		setClientsideBuffer();
		glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_handle);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer_size, buffer_data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		dirty = false;
	}
}

void Camera::setClientsideBuffer()
{
	//Even though these are vec3's, it will align each to take up 16 bytes
	buffer_data[0] = view_direction.x;
	buffer_data[1] = view_direction.y;
	buffer_data[2] = view_direction.z;
	buffer_data[3] = 0.0;

	buffer_data[4] = up_direction.x;
	buffer_data[5] = up_direction.y;
	buffer_data[6] = up_direction.z;
	buffer_data[7] = 0.0;

	buffer_data[8] = location.x;
	buffer_data[9] = location.y;
	buffer_data[10] = location.z;
	buffer_data[11] = 0.0;

	//Frustum elements
	buffer_data[12] = left;
	buffer_data[13] = right;
	buffer_data[14] = bottom;
	buffer_data[15] = top;
	buffer_data[16] = near_plane;
	buffer_data[17] = far_plane;
}

void Camera::initialize()
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
	buffer_data = new float[block_buffer_size / 4];

	for (int i = 0; i < block_buffer_size / 4; i++)
		buffer_data[i] = 0;
	//Generate the buffer object 
	glGenBuffers(1, &uniform_buffer_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_handle);
	//Allocate 'block_buffer_size' bytes of data on the GPU
	glBufferData(GL_UNIFORM_BUFFER, block_buffer_size, nullptr, GL_STATIC_DRAW);

	setClientsideBuffer();
	//Even though these are vec3's, it will align each to take up 16 bytes
	//data[0] = view_direction.x;
	//data[1] = view_direction.y;
	//data[2] = view_direction.z;
	//data[3] = 0.0;

	//data[4] = up_direction.x;
	//data[5] = up_direction.y;
	//data[6] = up_direction.z;
	//data[7] = 0.0;

	//data[8] = location.x;
	//data[9] = location.y;
	//data[10] = location.z;
	//data[11] = 0.0;

	////Frustum elements
	//data[12] = left;
	//data[13] = right;
	//data[14] = bottom;
	//data[15] = top;
	//data[16] = near_plane;
	//data[17] = far_plane;

	//Now buffer the data
	glBufferSubData(GL_UNIFORM_BUFFER, 0, block_buffer_size, buffer_data);
	if (glGetError() != GL_NO_ERROR)
		std::cout << "Uniform Block buffer error" << std::endl;
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniform_buffer_handle, 0, block_buffer_size);
	GLenum errc = glGetError();
	if (errc != GL_NO_ERROR)
		std::cout << errc << std::endl << "Uniform Block buffer error" << std::endl;

	//delete[] data;
	//Unbind uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	dirty = false;
	initialized = true;
}

void Camera::mouseChanged(int dx, int dy)
{
	view_direction.rotate(up_direction, ( (float) dx ) / 150.0);
	vec3 right = view_direction.cross(up_direction);
	up_direction.rotate(right, ( (float) -dy) / 250.0);
	view_direction.rotate(right, ( (float) -dy) / 250.0);
	view_direction.normalize();
	up_direction.normalize();
	dirty = true;
}

void Camera::setDirection(vec3 direction)
{
	view_direction = direction;
	dirty = true;
}

void Camera::setLocation(vec3 location)
{
	this->location = location;
	dirty = true;
}

void Camera::setUpDirection(vec3 up_direction)
{
	up_direction = up_direction;
	dirty = true;
}