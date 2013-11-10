#pragma once
#include <vector>
#include <hash_map>
#include "ObjFileReader.h"
#include "Texture3D.h"
#include "FrameBufferTexture.h"


class ResourceHandler
{
private:
	std::vector<ObjFileReader> meshes;
	std::vector<Texture> textures;
	std::vector<Shader> shader_programs;
	std::vector<BufferManager*> memory_objects;
	//std::hash_map<std::string, ObjFileReader*> obj_file_readers;
	std::hash_map<std::string, BufferManager> memory_map;
	std::hash_map<std::string, ObjFileReader*> mesh_map;
	std::hash_map<std::string, Shader> shader_map;
	std::hash_map<std::string, Texture*> texture_map;
public:
    //Returns a pointer to a Shader if filepaths is concatenated files of a VS/PS pair
    Shader getShaderByKey(std::string filepaths);
    //Returns a pointer to a BufferManager if filepaths is location of a valid .obj file
    BufferManager getBufferByKey(std::string filepaths);
    ObjFileReader* getFileReader(std::string filepath);
    //Returns a pointer to a Texture if filepaths is filepath to a texture
    Texture getTextureByKey(std::string filepaths);

    BufferManager* loadTransformBuffer(std::string filepath);
    FrameBufferTexture* loadFramebufferTexture(int left, int bottom, int width, int height, std::string name);
    Texture3D* load3DTexture(std::string filepath);
	Texture loadTexture(std::string img_filepath);
	ObjFileReader* loadMesh(std::string filepath);
	void loadShader(std::string vs_path, std::string fs_path);
	ObjectState synthesizeObject(std::string shader_key, std::string mesh_key, std::string texture);
	ResourceHandler(void);
	~ResourceHandler(void);
};

