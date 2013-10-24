

#version 430

//The fourth component of each light source is interpreted as it's intensity
uniform light_sources
{
	vec4 light_source_1;
	vec4 light_source_2;
};

uniform vec3 translation;
uniform vec3 scale;


//Assuming a symmetric view frustum
layout (std140)uniform  view_frustum
{
	float right;
	float top;
	float near;
	float far;
};

in vec3 position;
in vec3 normal; 
in vec2 texcoords;

out vec3 n_out;
out vec2 tex_out;
out float diffuse_component;

void main(void)
{
	mat4 modelview_matrix = {
		vec4(scale.x, 0.0, 0.0, translation.x),
		vec4(0.0, scale.y, 0.0, translation.y),
		vec4(0.0, 0.0, scale.z, translation.z),
		vec4(0.0, 0.0, 0.0, 1.0)};
	modelview_matrix = transpose(modelview_matrix);

	mat4 normal_transform = transpose(modelview_matrix);
	normal_transform = inverse(normal_transform);

	mat4 perspective_proj = {
		vec4(near / right, 0.0, 0.0, 0.0),
		vec4(0.0, near / top, 0.0, 0.0),
		vec4(0.0, 0.0, -(far + near) / (far - near), -2.0 * far * near / (far - near)),
		vec4(0.0, 0.0, -1.0, 0.0)};
	perspective_proj = transpose(perspective_proj);

	n_out = normal;
	tex_out = texcoords;
	vec4 worldspace_posn = modelview_matrix * vec4(position, 1.0);
	vec4 non_normalized = perspective_proj * worldspace_posn ;
	gl_Position = non_normalized / non_normalized.w;

	worldspace_posn /= worldspace_posn.w;
	//Calculate the world-space coordinates of the normal vector
	vec4 world_normal = normal_transform * vec4(normal, 1.0);

	vec4 nv = worldspace_posn / worldspace_posn.w;
	vec4 norm = world_normal;

	vec3 world_posn = vec3(nv.x, nv.y, nv.z);
	vec3 world_norm = vec3(norm.x, norm.y, norm.z);

	//Calculate diffuse and specular reflections
	vec3 unit_world_norm = normalize(world_norm);
	vec3 unit_world_posn = normalize(world_posn);

	//Simple diffuse component with no lights taken into account
	float lambertian = abs(dot(unit_world_norm, unit_world_posn));

	vec3 light_posn = vec3(light_source_1.x,  light_source_1.y, light_source_1.z);
	vec3 light_vec = light_posn - world_posn;

	light_vec = normalize(light_vec); 
	float specular = pow(dot(light_vec, unit_world_norm), 30) * 2;

	diffuse_component = (lambertian + specular) / 2.0;
}