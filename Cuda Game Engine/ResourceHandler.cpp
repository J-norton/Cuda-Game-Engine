#include "ResourceHandler.h"


ResourceHandler::ResourceHandler(void) :
	meshes(std::vector<ObjFileReader>()),
	textures(std::vector<Texture>()),
	shader_programs(std::vector<Shader>()),
	memory_objects(std::vector<BufferManager*>()),
	mesh_map(std::hash_map<std::string, ObjFileReader*>()),
	shader_map(std::hash_map<std::string, Shader>()),
	texture_map(std::hash_map<std::string, Texture*>())
{
}


ResourceHandler::~ResourceHandler(void)
{
}

ObjFileReader* ResourceHandler::loadMesh(std::string filepath)
{
	ObjFileReader* to_return = new ObjFileReader(filepath);
	mesh_map.insert( std::pair<std::string, ObjFileReader*>(filepath, to_return) );
	//memory_map.insert( std::pair<std::string, BufferManager>(filepath, (mesh_map[filepath]->getBuffers()) ) );
	//TODO: Debug check
	std::string s = memory_map[filepath].getType();
	return to_return;
}

ObjFileReader* ResourceHandler::getFileReader(std::string filepath)
{
	if (!mesh_map[filepath])
		return loadMesh(filepath);
	else
		return mesh_map[filepath];
}


void ResourceHandler::loadShader(std::string vs_path, std::string fs_path)
{
	shader_map.insert( std::pair<std::string, Shader>(vs_path.append(fs_path), Shader(vs_path, fs_path) ) );
}


//Known bug:
//When loading textures that are requested dynamically, that is,
//when loading a texture that is not specified in the Content Resources
//file, after doing the insert, the pointer is null and causes an error
//on the return statement.
Texture ResourceHandler::loadTexture(std::string img_filepath)
{
	texture_map.insert(std::pair<std::string, Texture*>( img_filepath, new Texture(img_filepath) ) );
	return *texture_map[img_filepath];
}


Texture3D* ResourceHandler::load3DTexture(std::string img_filepath)
{
	Texture3D* ptr = new Texture3D(img_filepath);
	texture_map.insert(std::pair<std::string, Texture3D*>( img_filepath, ptr ) );
	return ptr;
}

FrameBufferTexture* ResourceHandler::loadFramebufferTexture(int left, int bottom, int width, int height, std::string name)
{
	FrameBufferTexture* to_return = new FrameBufferTexture(left, bottom, width, height);
	texture_map.insert(std::pair<std::string, FrameBufferTexture*>( name,  to_return) );
	return to_return;
}

//Returns Shader where filepath key is the 'file location of VS'.append('FS file location')
Shader ResourceHandler::getShaderByKey(std::string filepath_key)
{
	Shader to_return;
	if (shader_map.find(filepath_key) != shader_map.end())
		to_return = shader_map[filepath_key];
	else
	{
		std::cout << "Invalid paths to identify shader" << std::endl;
		std::cout << "Shaders pairs are loaded from a resource .xml file." << std::endl;
		std::cout << "The key for a shader pair is 'path to VS''path to FS'" << std::endl;
		std::cout << "Check the resource .xml file to ensure that the pair is loaded together." << std::endl;
	}

	return to_return;
}

//Returns Buffers read from the .obj file at filepath_key
BufferManager ResourceHandler::getBufferByKey(std::string filepath_key)
{
	BufferManager* to_return = nullptr;
	//   if (memory_map[filepath_key])
	//{
	//       to_return = memory_map[filepath_key];
	//}
	//   else
	//       std::cout << "Invalid buffer key" << std::endl;

	//return memory_map[filepath_key];
	return ObjFileReader(filepath_key).getBuffers();
}

//Returns Texture read from the file at filepath_key
Texture ResourceHandler::getTextureByKey(std::string filepath_key)
{
	Texture* to_return = nullptr;
	if (texture_map[filepath_key])
		to_return = texture_map[filepath_key];
	else
	{
		//If the texture has not yet been loaded, then issue a warning and try to
		//load the texture.
		std::cout << "Invalid texture key " << filepath_key << " has not yet been loaded"  << std::endl;
		std::cout << "Loading new texture and adding to cache"  << std::endl;
		loadTexture(filepath_key);
		to_return = texture_map[filepath_key];
	}

	return *to_return;
}

BufferManager* ResourceHandler::loadTransformBuffer(std::string filepath)
{
	BufferManager* to_add = new TransformFeedbackBuffer(filepath);
	memory_objects.push_back(to_add);
	return to_add;
}