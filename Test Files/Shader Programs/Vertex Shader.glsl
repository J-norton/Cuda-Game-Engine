
#version 430

//The fourth component of each light source is interpreted as it's intensity
layout (std140) uniform light_sources
{
	vec4 light_source_1;
};

//Position information. rotation.x/y/z is the rotation in that direction. No order is specified.
uniform vec3 translation;
uniform vec3 scale;
uniform vec3 rotation;

layout (std140) uniform camera_attributes
{
	vec4 view_direction;
	vec4 up_direction;
	vec4 location;
	//The following two items hold the information for the view frustum
	//This holds four distinct elements, the left, right, bottom and top planes
	vec4 lrbt;
	//This holds the near and far clipping planes in nf.x and nf.y
	vec4 nf;
};

in vec3 position;
in vec3 normal; 
in vec2 texcoords;

smooth out vec2 tex_out;
out float dummy_variable;
out flat int red_signal;
out float green_signal;
out float diffuse_component;
out float perspective_div;

const float PI = 3.14159265358979323846264;
mat4 getRotationMatrix();
mat4 getCameraMatrix();

void main(void)
{
	float left = lrbt.x;
	float right = lrbt.y;
	float bottom = lrbt.z;
	float top = lrbt.w;
	float near = nf.x;
	float far = nf.y;
	//mat4 rotation = getRotationMatrix();
	mat4 camera = getCameraMatrix();
	mat4 modelview_matrix = {
		vec4(scale.x, 0.0, 0.0, translation.x - location.x),
		vec4(0.0, scale.y, 0.0, translation.y - location.y),
		vec4(0.0, 0.0, scale.z, translation.z - location.z),
		vec4(0.0, 0.0, 0.0, 1.0)};
	modelview_matrix = transpose(modelview_matrix);
	modelview_matrix = camera * modelview_matrix;// * rotation;

	mat4 normal_transform = transpose(modelview_matrix);
	normal_transform = inverse(normal_transform);

	mat4 perspective_proj = {
		vec4(near / right, 0.0, 0.0, 0.0),
		vec4(0.0, near / top, 0.0, 0.0),
		vec4(0.0, 0.0, -(far + near) / (far - near), -2 * far * near / (far - near)),  
		vec4(0.0, 0.0, -1.0, 0.0)};
	perspective_proj = transpose(perspective_proj);

	vec4 worldspace_posn = modelview_matrix * vec4(position, 1.0);
	//if (worldspace_posn.z > 0.0)
	//	green_signal = 1.0;
	//else
	//	green_signal = 0.0;
	//vec4 worldspace_posn = vec4(position, 1.0) * modelview_matrix;

	//worldspace_posn /= worldspace_posn.w;
	vec4 non_normalized = perspective_proj * worldspace_posn ;
	if (0.0 > non_normalized.w)
		green_signal = 1;
	else
		green_signal = 0;
	float perspective_w = abs(non_normalized.w);
	non_normalized = non_normalized / abs(non_normalized.w);
	tex_out = texcoords;
	perspective_div = perspective_w;

	gl_Position = non_normalized;

	//Calculate the world-space coordinates of the normal vector
	vec4 world_normal = normal_transform * vec4(normal, 0.0);

	vec3 world_posn = vec3(worldspace_posn.x, worldspace_posn.y, worldspace_posn.z);
	vec3 world_norm = vec3(world_normal.x, world_normal.y, world_normal.z);

	//Calculate diffuse and specular reflections
	vec3 unit_world_norm = -normalize(world_norm);
	vec3 unit_world_posn = normalize(world_posn);

	//Simple diffuse component with no lights taken into account
	float lambertian = dot(unit_world_norm, unit_world_posn);

	vec3 light_posn = vec3(light_source_1.x,  light_source_1.y, light_source_1.z);
	vec3 light_vec = light_posn - world_posn;

	light_vec = normalize(light_vec); 

	float blinn_diffuse = abs(dot( unit_world_posn, light_vec) );

	vec3 h_vector = light_vec - unit_world_posn;
	float test = dot(h_vector, unit_world_norm);

	//signal turns the triangle red if set
	//if (test < -0.01)
	//	signal = 1;
	//else
	//	signal = 0;

	float specular = pow(dot(light_vec, unit_world_norm), 2.0);

	float intensity = lambertian;// + blinn_diffuse + specular;
	dummy_variable = specular;
	diffuse_component = intensity;// / 2.0;
}

mat4 getCameraMatrix()
{
	vec3 up = vec3(up_direction.x, up_direction.y, up_direction.z);
	vec3 view = vec3(view_direction.x, view_direction.y, view_direction.z);

	vec3 right_direction = cross(up, view);
	mat4 to_return = {
		vec4(right_direction, 0.0),
		up_direction,
		view_direction,
		vec4(0.0, 0.0, 0.0, 1.0)};
	to_return = transpose(to_return);

	return to_return;
}

mat4 getRotationMatrix()
{
	mat4 x_comp = {
		vec4(1, 0.0, 0.0, 0.0),
		vec4(0.0, cos(rotation.x * PI), -sin(rotation.x * PI), 0.0),
		vec4(0.0, sin(rotation.x * PI), cos(rotation.x * PI), 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)};
	x_comp = transpose(x_comp);

	mat4 y_comp = {
		vec4(cos(rotation.y * PI), 0.0, sin(rotation.y * PI), 0.0),
		vec4(0.0, 1.0, 0.0, 0.0),
		vec4(-sin(rotation.y * PI), 0.0, cos(rotation.y * PI), 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)};
	y_comp = transpose(y_comp);


	mat4 z_comp = {
		vec4(cos(rotation.z * PI), -sin(rotation.z * PI), 0.0, 0.0),
		vec4(sin(rotation.z * PI), cos(rotation.z * PI), 0.0, 0.0),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)};
	z_comp = transpose(z_comp);

	return y_comp * z_comp * x_comp;
}