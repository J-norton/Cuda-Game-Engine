#include "vec3.hpp"

vec3 operator+(vec3 const& a, vec3 const& b)
{
	vec3 to_return = vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	return to_return;
}

vec3 vec3::operator* (float m)
{
	return vec3(x * m, y * m, z * m);
}

vec3 vec3::operator-()
{
	return vec3(-x, -y, -z);
}

float vec3::magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

void vec3::normalize()
{
	float mag = magnitude();
	*this = (*this) * (1.0 / mag);
}

vec3 vec3::cross(vec3 const& b)
{
	return vec3( y * b.z - z * b.y , z * b.x - x * b.z , x * b.y - y * b.x );
}

vec3::vec3(float x, float y, float z) :
	x(x),
	y(y),
	z(z)
{
}

vec3::vec3() :
	x(0),
	y(0),
	z(0)
{
}

void vec3::rotate(vec3 axis, float theta)
{
	float gam = cos(theta);
	float sig = sin(theta);

	vec3 row_1 = vec3(gam + (1 - gam) * pow(axis.x, 2.0f),
		-axis.z * sig + (1 - gam) * axis.y * axis.x, axis.y * sig + (1 - gam) * axis.x * axis.z);

	vec3 row_2 = vec3(axis.z * sig + (1 - gam) * axis.y * axis.x,
		gam + (1 - gam) * pow(axis.y, 2.0f), -axis.x * sig + (1 - gam) * axis.y * axis.z);

	vec3 row_3 = vec3(-axis.y * sig + (1 - gam) * axis.x * axis.z,
		axis.x * sig + (1 - gam) * axis.y * axis.z, gam + (1 - gam) * pow(axis.x, 2.0f));

	float tmp_x = (*this) * row_1;
	float tmp_y = (*this) * row_2;
	float tmp_z = (*this) * row_3;

	x = tmp_x;
	y = tmp_y;
	z = tmp_z;
}

float operator*(vec3 const& a, vec3 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


