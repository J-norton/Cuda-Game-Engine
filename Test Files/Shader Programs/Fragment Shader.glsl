#version 430

smooth in vec2 tex_out;
in float diffuse_component;
in float dummy_variable;
in flat int red_signal;
in float green_signal;
in float perspective_div;

uniform sampler2D in_texture;

out vec4 output_color;

void main(void)
{
	
	//if (gl_Position > 0.0)
	//	discard;
	vec4 raw_color = diffuse_component * texture(in_texture, tex_out );
	//if (diffuse_component == 0.0)
	//	raw_color = vec4(0.0, 1.0, 0.0, 1.0);
	//gl_FragColor = pow(raw_color, vec4(1.0 / 2.2));
	if (red_signal != 1 && (abs(green_signal - 1.0f) > 0.01) )
		output_color = pow(raw_color, vec4(1.0 / 2.2));

	if (dummy_variable == 0.23820)
		output_color = vec4(1.0, 1.0, 0.0, 1.0);
	if (red_signal == 1)
	{
		//discard;
		output_color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	if (abs(green_signal - 1.0f) <= 0.01)
		output_color = vec4(0.0, 1.0, 0.0, 1.0);
	if (red_signal == 1 && (abs(green_signal - 1.0f) <= 0.01))
		output_color = vec4(1.0, 1.0, 0.0, 1.0);
}