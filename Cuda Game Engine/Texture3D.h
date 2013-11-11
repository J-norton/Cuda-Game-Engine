#pragma once
#include "Texture.h"

class Texture3D :
    public Texture
{
public:
	virtual void setIntParameter(int enum_type, int parameter);
	virtual void setFloatParameter(int enum_type, float parameter);
    virtual void bind();
	virtual void unbind();
    Texture3D(const Texture3D& copy);
    Texture3D& operator=(const Texture3D& rhs);
    Texture3D(std::string filepath);
    Texture3D(void);
    ~Texture3D(void);
};

