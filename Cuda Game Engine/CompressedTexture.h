#pragma once
#include "Texture.h"
class CompressedTexture :
	public Texture
{
public:
	CompressedTexture(std::string);
	CompressedTexture(void);
	~CompressedTexture(void);
};

