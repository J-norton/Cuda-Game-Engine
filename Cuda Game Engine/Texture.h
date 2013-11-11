#pragma once
#include <string>
#include <Magick++.h>
#include "WorldState.h"
#include <iostream>
#include "Shader.h"
#include <list>


class Texture
{
protected:
	Magick::Image image;
	static int texture_count;
	int texture_num;
	unsigned int handle;
	int texture_type;
	int tex_width;
	int tex_height;

    static int maximum_texture_units;

public:
	virtual void setIntParameter(int enum_type, int parameter);
	virtual void setFloatParameter(int enum_type, float parameter);
	virtual void bind();
	virtual void unbind();
	//Constructs a texture from an image file
	Texture(std::string file_path, bool generate_mipmaps = false);
	//Constructs a texture from an image file with specified size
	Texture(std::string file_path, int width, int height);
	//Constructs a texture from a pre-existing image
	Texture(Magick::Image texture);
	Texture(void);
	~Texture(void);
};

