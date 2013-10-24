#pragma once
#include "TriangleMesh.h"

class ErrorMessage :
	public TriangleMesh
{
private:
	std::string error_message;
	Texture error_texture ;
    int error_id;
    //int test;
public:
	void setText(std::string message);
	ErrorMessage(std::string error_message);
	ErrorMessage(void);
	~ErrorMessage(void);
};

