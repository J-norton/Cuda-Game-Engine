#version 430

in vec3 n_out;
in vec2 tex_out;
in float diffuse_component;

uniform sampler2D in_texture;

void main(void)
{
	float r = 0.6;
	if (n_out.x == 3.0)
		r = 0.7;
	if (tex_out.x == 3.0)
		r = 0.7;

    if (n_out.x == 328892.2)
		gl_FragColor = vec4(r, 0.0, 0.8, 1.0);
	else
	{
		vec4 raw_color = diffuse_component * texture(in_texture, tex_out);
		gl_FragColor = pow(raw_color, vec4(1.0 / 2.2));
	}

}