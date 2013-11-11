#pragma once
#include "Texture.h"
class MipmappedTexture :
    public Texture
{
public:
    MipmappedTexture(std::string filepath);
    MipmappedTexture(void);
    ~MipmappedTexture(void);
};

