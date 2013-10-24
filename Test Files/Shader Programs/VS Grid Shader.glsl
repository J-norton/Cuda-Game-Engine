

#version 430
in vec3 position;
out vec3 screen_posn;
void main(void)

{
	gl_Position = vec4(position, 1.0);
}