#version 430

in vec2 tex_out;
in float diffuse_component;
in float dummy_variable;
in float red_signal;
in float green_signal;

uniform sampler2D in_texture;

out vec4 output_color;

void main(void)
{
	vec2 positive_texcoords = vec2(abs(tex_out.x), abs(tex_out.y));
	vec4 raw_color = diffuse_component * texture(in_texture, positive_texcoords) + vec4(red_signal, 0.0, 0.0, 1.0);
	//if (diffuse_component == 0.0)
	//	raw_color = vec4(0.0, 1.0, 0.0, 1.0);
	//gl_FragColor = pow(raw_color, vec4(1.0 / 2.2));


	if (red_signal != 1.0 && (abs(green_signal - 1.0f) > 0.01) )
		output_color = pow(raw_color, vec4(1.0 / 2.2));
	if (diffuse_component < 0.001f)
		output_color = vec4(1.0f, 0.0f, 1.0f, 1.0f);

	if (dummy_variable == 0.23820)
		output_color = vec4(1.0, 1.0, 0.0, 1.0);
	if (red_signal == 1.0)
		output_color = vec4(1.0, 0.0, 0.0, 1.0);
	if (abs(green_signal - 1.0f) <= 0.01)
		output_color = vec4(0.0, 1.0, 0.0, 1.0);
	if (red_signal == 1.0 && (abs(green_signal - 1.0f) <= 0.01))
		output_color = vec4(1.0, 1.0, 0.0, 1.0);
}