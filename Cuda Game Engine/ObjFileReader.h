#pragma once
#include <string.h>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "CudaGLBuffer.cuh"
#include <sstream>
#include "vec3.hpp"

struct face
{
	int indices[3];
};

struct IndexBuffer
{
	std::vector<face> vertex_face;
	std::vector<face> normal_face;
	std::vector<face> texcoords_face;
	float* raw_v_data;
	float* raw_n_data;
	int* raw_i_data;
	float* raw_vt_data;
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
	IndexBuffer all_buffers;
	std::vector<face> vertex_face;
	std::vector<face> normal_face;
	std::vector<face> texcoords_face;
	std::vector<std::pair<IndexBuffer, std::string> > component_indices;
	std::unordered_map<std::string, bool> mtl_names;
	//The number of textures referenced by the obj file
	int num_materials;
	

	float* raw_v_data;
	float* raw_n_data;
	float* raw_vt_data;
	int* raw_i_data;

	BufferManager buffers;
	int poly_count;

	//Calculates normals for each triangle if the obj file doesn't provide them
	void ensureNormals(IndexBuffer&);
	void ensureNormalsFlat(IndexBuffer&);
    //If the obj file has no indices, this synthesizes them.
    void ensureIndices();
    //Fills texture coordinates with zeros if obj file doesn't provide them
    void ensureTexcoords();
    //Places texture coordinates where they can be accessed by an index buffer
	void setVertexIndices(IndexBuffer&);
	//duplicates vertices in place of using an index buffer
	void setVerticesFlat(IndexBuffer& );
	BufferManager bufferMemory(IndexBuffer& ibo);
	void loadObjModel(std::string model_path);
	float* processData(std::string data);

	void tokenize(std::string& to_break, std::vector<std::string>& tokens, const char* delimiter);

public:
	//Returns the indexth geometric primitive comprising the object
	BufferManager getPrimitive(int index);
	//Returns how many smaller geometric shapes the obj file was broken into
	int getNumSubMeshes();
	BufferManager getBuffers();
	//Returns the interleaved buffer object for a single material
	BufferManager getNextBuffer();
	ObjFileReader(std::string path);
	ObjFileReader(void);
	~ObjFileReader(void);
};

