
#pragma once
#include <math.h>

struct vec3
{
	float x;
	float y;
	float z;
	vec3(float x, float y, float z);
	vec3();
	friend vec3 operator+(vec3 const& a, vec3 const& b);
	vec3 operator*(float b);
	vec3 operator-();
	vec3 cross(vec3 const& b);
	friend float operator*(vec3 const& a, vec3 const& b);
	void rotate(vec3 axis, float theta);
	float magnitude() const;
	void normalize();
};


