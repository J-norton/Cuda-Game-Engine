#include "Texture3D.h"

Texture3D::Texture3D(std::string filepath) 
{
    texture_type = GL_TEXTURE_3D;
	if (glGetError() != 0)
		std::cout << glGetError() << std::endl << "Illegal arguments set for texture";
	std::string dir_name = "../Test Files/";
	filepath = dir_name.append(filepath);
	texture_num = GL_TEXTURE0 + texture_count;
	try {
		image.read(filepath);
	}
	catch (Magick::WarningCoder& warning)
	{
		std::cerr << "Magick WarningCoder: " << warning.what();
	}
	catch (Magick::Warning& warning)
	{
		std::cerr << "Magick Warning: " << warning.what() << std::endl;
	}

	catch (Magick::Exception& error)
	{
		std::cout << "Error loading image\n" << error.what() << std::endl;
	}
	catch( std::exception &error ) 
	{ 
		// Process any other exceptions derived from standard C++ exception
		std::cerr << "Caught C++ STD exception: " << error.what() << std::endl;
	} 
	tex_width = image.columns();
	tex_height = image.rows();
	Magick::PixelPacket* pixel = image.getPixels(0, 0, tex_width, tex_height);

	//Set pixel storage parameters
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glActiveTexture(texture_num);
	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_3D, handle);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLubyte* raw_data = new GLubyte[tex_width * tex_height * 4];

	int k = 0;
	for (int i = 0; i < tex_height * tex_width; i++)
	{
		int r, g, b, a;
		r = pixel[i].red;
		g = pixel[i].green;
		b = pixel[i].blue;
		a = pixel[i].opacity;

		raw_data[k++] = pixel[i].red;
		raw_data[k++] = pixel[i].green;
		raw_data[k++] = pixel[i].blue;
		raw_data[k++] = pixel[i].opacity;
	}

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, tex_width, tex_height, 1,
		0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);


	glBindTexture(GL_TEXTURE_3D, 0);
	delete[] raw_data;
	texture_count++;
	texture_count %= 16;
}

void Texture3D::bind()
{
	//glEnable(texture_type);
	int tex_handle = WorldState::getCurrentShader()->getUniform("texture");
	if (tex_handle == -1)
	{
		std::cout << "No texture named 'texture' in the current shader" << std::endl;
		return;
	}
	glActiveTexture(texture_num);

	glBindTexture(GL_TEXTURE_3D, handle);
	glUniform1i(tex_handle, texture_num - GL_TEXTURE0);
}

//Sets an integer texture parameter of type, enum_type and value, parameter
void Texture3D::setIntParameter(int enum_type, int parameter)
{
	glActiveTexture(texture_num);
	glBindTexture(GL_TEXTURE_3D, handle);
	glTexParameteri(GL_TEXTURE_3D, enum_type, parameter);
	if (glGetError() != 0)
		std::cout << "Illegal arguments set for texture";

	glBindTexture(GL_TEXTURE_3D, 0);
}

Texture3D::Texture3D(const Texture3D& copy)
{
    texture_type = GL_TEXTURE_3D;
    handle = copy.handle;
    texture_num = copy.texture_num;
    tex_width = copy.tex_width;
    tex_height = copy.tex_height;
}

Texture3D& Texture3D::operator=(const Texture3D& rhs)
{
    texture_type = GL_TEXTURE_3D;
    handle = rhs.handle;
    texture_num = rhs.texture_num;
    tex_width = rhs.tex_width;
    tex_height = rhs.tex_height;
    return *this;
}

//Sets a float texture parameter of type, enum_type and value, parameter
void Texture3D::setFloatParameter(int enum_type, float parameter)
{
	glActiveTexture(texture_num);
	glBindTexture(GL_TEXTURE_3D, handle);
	glTexParameterf(GL_TEXTURE_3D, enum_type, parameter);
	if (glGetError() != 0)
		std::cout << "Illegal arguments set for texture";

	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::unbind()
{
    //if (prev != nullptr)
    //    prev->bind();
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);
}

Texture3D::Texture3D(void)
{
    texture_type = GL_TEXTURE_3D;
    handle = 0;
    texture_num = 0;
    tex_width = 0;
    tex_height = 0;
}


Texture3D::~Texture3D(void)
{
}
