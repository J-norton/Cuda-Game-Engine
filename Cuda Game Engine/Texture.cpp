#include "Texture.h"

int Texture::texture_count = 0;
int Texture::maximum_texture_units = 0;

Texture::Texture(std::string filepath, bool generate_mipmaps) :
	texture_type(GL_TEXTURE_2D)
{
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &Texture::maximum_texture_units);
	if (glGetError() != 0)
		std::cout << glGetError() << std::endl << "Illegal arguments set for texture";
	//std::string dir_name = "../Test Files/";
	std::string dir_name = "C:/Users/James/Documents/Visual Studio 2012/Projects/Cuda Game Engine/Test Files/Textures/";
	filepath = dir_name.append(filepath);
	texture_num = GL_TEXTURE0 + texture_count;
	try {

		std::string srcdir("");
		std::list<Magick::Image> imageList;
		//TODO: Fix this so it is not terrible and uses a relative path
		Magick::readImages( &imageList, dir_name);
		image = imageList.back();

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
	image.flip();
	tex_width = image.columns();
	tex_height = image.rows();
	Magick::PixelPacket* pixel = image.getPixels(0, 0, tex_width, tex_height);

	//Set pixel storage parameters
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glActiveTexture(texture_num);
	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);

	if (generate_mipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

	//glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 2);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);


	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] raw_data;
	texture_count++;
	texture_count %= Texture::maximum_texture_units;
}


//Texture::Texture(std::string filepath, int width, int height) :
//	texture_type(GL_TEXTURE_2D)
//{
//	if (glGetError() != 0)
//		std::cout << glGetError() << std::endl << "Illegal arguments set for texture";
//	std::string dir_name = "../Test Files/";
//	filepath = dir_name.append(filepath);
//	texture_num = GL_TEXTURE0 + texture_count;
//	try {
//		image.read(filepath);
//	}
//	catch (Magick::WarningCoder& warning)
//	{
//		std::cerr << "Magick WarningCoder: " << warning.what();
//	}
//	catch (Magick::Warning& warning)
//	{
//		std::cerr << "Magick Warning: " << warning.what() << std::endl;
//	}
//
//	catch (Magick::Exception& error)
//	{
//		std::cout << "Error loading image\n" << error.what() << std::endl;
//	}
//	catch( std::exception &error ) 
//	{ 
//		// Process any other exceptions derived from standard C++ exception
//		std::cerr << "Caught C++ STD exception: " << error.what() << std::endl;
//	} 
//	tex_width = width;
//	tex_height = height;
//	Magick::PixelPacket* pixel = image.getPixels(0, 0, tex_width, tex_height);
//
//	//Set pixel storage parameters
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//	glActiveTexture(texture_num);
//	glGenTextures(1, &handle);
//
//	glBindTexture(GL_TEXTURE_2D, handle);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	GLubyte* raw_data = new GLubyte[tex_width * tex_height * 4];
//
//	int k = 0;
//	for (int i = 0; i < tex_height * tex_width; i++)
//	{
//		int r, g, b, a;
//		r = pixel[i].red;
//		g = pixel[i].green;
//		b = pixel[i].blue;
//		a = pixel[i].opacity;
//
//		raw_data[k++] = pixel[i].red;
//		raw_data[k++] = pixel[i].green;
//		raw_data[k++] = pixel[i].blue;
//		raw_data[k++] = pixel[i].opacity;
//	}
//
//	//glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 2);
//	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 2);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);
//
//	if (glGetError() != 0)
//		std::cout << "Illegal arguments set for texture";
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	delete[] raw_data;
//	texture_count++;
//	int num;
//	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &num);
//	texture_count %= num;
//}

Texture::Texture(Magick::Image texture)
{
	texture_num = GL_TEXTURE0 + texture_count;
	image = texture;
	tex_width = image.columns();
	tex_height = image.rows();
	Magick::PixelPacket* pixel = image.getPixels(0, 0, tex_width, tex_height);

	//Set pixel storage parameters
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glActiveTexture(texture_num);
	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLubyte* raw_data = new GLubyte[tex_width * tex_height * 4];

	int k = 0;
	for (int i = 0; i < tex_height * tex_width; i++)
	{
		int r, g, b, a;
		r = pixel[i].red;
		g = pixel[i].green;
		b = pixel[i].blue;
		//a = pixel[i].opacity;
		a = 65535;

		raw_data[k++] = r;
		raw_data[k++] = g;
		raw_data[k++] = b;
		raw_data[k++] = a;
	}

	//glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 2);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height,
		1, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);

	if (glGetError() != 0)
		std::cout << "Illegal arguments set for texture";

	//glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, tex_width * 2, tex_height * 2,
	//	0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);

	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] raw_data;
	texture_count++;
	texture_count %= 16;
}


//Passes parameters to the current instance of texture via glTexParameteri
void Texture::setIntParameter(int enum_type, int parameter_value)
{
	glActiveTexture(texture_num);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, enum_type, parameter_value);
	if (glGetError() != 0)
		std::cout << "Illegal arguments set for texture";

	glBindTexture(GL_TEXTURE_2D, 0);
}

//Passes parameter to the current instance of texture via glTexParameterf
void Texture::setFloatParameter(int enum_type, float parameter_value)
{
	glActiveTexture(texture_num);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameterf(GL_TEXTURE_2D, enum_type, parameter_value);
	if (glGetError() != 0)
		std::cout << "Illegal arguments set for texture";

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(void)
{
}


Texture::~Texture(void)
{
}


void Texture::bind()
{
	//glEnable(GL_TEXTURE_2D);
	int tex_handle = WorldState::getCurrentShader()->getUniform("in_texture");
	if (tex_handle == -1)
	{
		std::cout << "No texture named 'texture' in the current shader" << std::endl;
		return;
	}
	glActiveTexture(texture_num);

	glBindTexture(GL_TEXTURE_2D, handle);
	glUniform1i(tex_handle, texture_num - GL_TEXTURE0);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

