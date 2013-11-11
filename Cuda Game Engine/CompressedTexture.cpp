#include "CompressedTexture.h"

CompressedTexture::CompressedTexture(std::string filepath) 
{
    texture_type = GL_TEXTURE_2D;
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

    glBindTexture(GL_TEXTURE_2D, handle);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RED, tex_width, tex_height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);
    if (glGetError() != 0)
        std::cout << glGetError() << std::endl << "Illegal arguments set for texture";

    GLboolean compressed;
    GLint flag;
    GLenum format;

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &flag);
    if (flag == GL_TRUE)
        std::cout << "Compression set" << std::endl;
    if (glGetError() != 0)
        std::cout << glGetError() << std::endl << "Illegal arguments set for texture";

    //glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height,
    //	0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw_data);


    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] raw_data;
    texture_count++;
    texture_count %= 16;
}

CompressedTexture::CompressedTexture(void)
{
}


CompressedTexture::~CompressedTexture(void)
{
}
