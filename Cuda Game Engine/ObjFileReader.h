#pragma once
#include <string.h>
#include <vector>
#include <fstream>
#include "CudaGLBuffer.cuh"
//#include <boost/algorithm/string.hpp>
#include <sstream>

struct vec3
{
	float x;
	float y;
	float z;
};
struct face
{
	int indices[3];
};

/**
*/
class ObjFileReader
{
private:
	float low_left[3];
	float top_right[3];
	std::string file_path;
	std::vector<vec3> vertex_data;
	std::vector<vec3> normal_data;
	std::vector<vec3> texcoords_data;
	std::vector<vec3> normal_r;
	std::vector<vec3> texcoords_r;
	std::vector<face> vertex_face;
	std::vector<face> normal_face;
	std::vector<face> texcoords_face;

	float* raw_v_data;
	float* raw_n_data;
	float* raw_vt_data;
	int* raw_i_data;

	CudaGLBufferObj buffers;
	int poly_count;

	//Calculates normals for each triangle if the obj file doesn't provide them
	void ensureNormals();
    //If the obj file has no indices, this synthesizes them.
    void ensureIndices();
    //Fills texture coordinates with zeros if obj file doesn't provide them
    void ensureTexcoords();
    //Places texture coordinates where they can be accessed by an index buffer
	void setVertexIndices();
	void bufferMemory();
	void loadObjModel(std::string model_path);
	float* processData(std::string data);

	void tokenize(std::string& to_break, std::vector<std::string>& tokens, const char* delimiter);

public:

	CudaGLBufferObj* getBuffers();
	ObjFileReader(std::string path);
	ObjFileReader(void);
	~ObjFileReader(void);
};

