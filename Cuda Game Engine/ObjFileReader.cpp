#include "ObjFileReader.h"


ObjFileReader::ObjFileReader(void)
{
}

ObjFileReader::ObjFileReader(std::string path) :
	file_path(path),
	vertex_data(std::vector<vec3>()),
	normal_data(std::vector<vec3>()),
	texcoords_data(std::vector<vec3>()),
	vertex_face(std::vector<face>()),
	normal_face(std::vector<face>()),
	texcoords_face(std::vector<face>()),
	normal_r(std::vector<vec3>()),
	texcoords_r(std::vector<vec3>()),
	poly_count(0)
{
	std::string dir = "../Test Files/Meshes/";
	loadObjModel(dir.append(path));
	ensureIndices();
	ensureNormals();
	ensureTexcoords();
	setVertexIndices();
	bufferMemory();
}

void ObjFileReader::ensureIndices()
{
	if (vertex_face.size() == 0)
	{
		for (int i = 0; i < vertex_data.size() - 3; i += 3)
		{
			face fv = {i + 1, i + 2, i + 3};
			vertex_face.push_back(fv);
		}
	}
}

void ObjFileReader::ensureTexcoords()
{
	if (texcoords_data.size() == 0)
	{
		for (int i = 0; i < vertex_data.size(); i++)
		{
			vec3 dummy_data = {0, 0, 0};
			texcoords_data.push_back(dummy_data);
		}
	}
	if (texcoords_face.size() == 0)
		texcoords_face = vertex_face;
}

void ObjFileReader::loadObjModel(std::string model_path)
{
	std::ifstream src_file = std::ifstream(model_path.c_str());

	if (!src_file.is_open())
	{
		printf("ERROR: Unable to load mesh from %s!\n", model_path.c_str());
		exit(0);
	}

	float xmin = FLT_MAX;
	float xmax = -FLT_MAX;
	float ymin = FLT_MAX;
	float ymax = -FLT_MAX;
	float zmin = FLT_MAX;
	float zmax = -FLT_MAX;

	std::vector<std::string> token1;
	std::vector<std::string> token2;
	std::vector<std::string> token3;
	std::vector<std::string> tokens;
	while (true)
	{
		char line[256] = {0};
		src_file.getline(line, 256);

		if (src_file.eof())
			break;

		if (strlen(line) <= 1)
			continue;
		tokens.clear();

		tokenize(std::string(line), tokens, " ");


		if (tokens[0] == "v")
		{
			//This code saves the bounding box for the mesh
			//float x = atof(tokens[1].c_str());
			//float y = atof(tokens[2].c_str());
			//float z = atof(tokens[3].c_str());
			//xmin = min(x, xmin);
			//xmax = max(x, xmax);
			//ymin = min(y, ymin);
			//ymax = max(y, ymax);
			//zmin = min(z, zmin);
			//zmax = max(z, zmax);

			vec3 position = {atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str())};
			vertex_data.push_back(position);
		}
		else if (tokens[0] == "vn")
		{
			vec3 normal = {atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str())};
			normal_data.push_back(normal);
		}
		//Texture coordinates
		else if (tokens[0] == "vt")
		{
			vec3 texcoord = {atof(tokens[1].c_str()), atof(tokens[2].c_str()), 0};
			texcoords_data.push_back(texcoord);
		}
		else if (tokens[0] == "f")
		{
			token1.clear();
			//token2.clear();
			//token3.clear();
			tokenize(tokens[1], token1, "/");

			//Critical path: This code is fucking slow. There is a lot of redundant work being done
			//as far as clearing token2 and token3. Only need to clear one of them per iteration
			//through the loop.

			// Then parse v, vt and vn data
			if (token1.size() == 3)
				for (unsigned int i = 2; i < tokens.size() - 1; i++)
				{
					//TODO: Don't retokenize token3
					token2.clear();
					token3.clear();
					tokenize(tokens[i], token2, "/");
					tokenize(tokens[i + 1], token3, "/");

					//Check to ensure that each newly read face has three items
					if (token2.size() < 3 || token3.size() < 3)
						continue;

					face v_face = { atoi(token1[0].c_str()), atoi(token2[0].c_str()), atoi(token3[0].c_str()) };
					face vt_face = { atoi(token1[1].c_str()), atoi(token2[1].c_str()), atoi(token3[1].c_str()) };
					face n_face = { atoi(token1[2].c_str()), atoi(token2[2].c_str()), atoi(token3[2].c_str()) };

					vertex_face.push_back(v_face);
					texcoords_face.push_back(vt_face);
					normal_face.push_back(n_face);
				}
			else if (token1.size() == 2)	//parse v and vt data
			{
				for (unsigned int i = 2; i < tokens.size() - 1; i++)
				{
					if (i == 2)
					{
						token2.clear();
						token3.clear();
						tokenize(tokens[i], token2, "/");
						tokenize(tokens[i + 1], token3, "/");
					}
					//Set token2 to token3 and set token3 to a newly parsed string
					else
					{
						token2 = token3;
						token3.clear();
						tokenize(tokens[i + 1], token3, "/");
					}
					if (token1.size() < 2 || token2.size() < 2 || token3.size() < 2)
						continue;

					face v_face = { atoi(token1[0].c_str()), atoi(token2[0].c_str()), atoi(token3[0].c_str()) };
					face vt_face = { atoi(token1[1].c_str()), atoi(token2[1].c_str()), atoi(token3[1].c_str()) };

					vertex_face.push_back(v_face);
					texcoords_face.push_back(vt_face);
				}
			}
			else if (token1.size() == 1)	//parse v data
			{
				for (unsigned int i = 2; i < tokens.size() - 1; i++)
				{
					token2.clear();
					token3.clear();
					tokenize(tokens[i], token2, "/");
					tokenize(tokens[i + 1], token3, "/");
					if (token1.size() < 1 || token2.size() < 1 || token3.size() < 1)
						continue;

					face v_face = { atoi(token1[0].c_str()), atoi(token2[0].c_str()), atoi(token3[0].c_str()) };

					vertex_face.push_back(v_face);
				}
			}
		}
	}
}

void ObjFileReader::ensureNormals()
{
	if (normal_data.size() == 0)
	{
		for (int i = 0; i < vertex_data.size(); i++)
		{
			vec3 to_put = {0, 0, 0};
			normal_data.push_back(to_put);
		}
		// First construct normal vectors from the vertex information
		for (face f : vertex_face)
		{
			vec3 v1 = vertex_data[f.indices[0] - 1];
			vec3 v2 = vertex_data[f.indices[1] - 1];
			vec3 v3 = vertex_data[f.indices[2] - 1];
			// a and b are the two vectors that point in the direction
			// of two of the sides of the triangle
			vec3 a = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
			vec3 b = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

			// Take the cross product of a and b to obtain the normal.
			vec3 c = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
			for (int j = 0; j < 3; j++)
				normal_data.at(f.indices[j] - 1) = c;
		}
		normal_face = vertex_face;
	}
	if (normal_face.size() == 0)
		normal_face = vertex_face;
}

void ObjFileReader::setVertexIndices()
{
	raw_v_data = new float[vertex_data.size() * 3];
	int k = 0;
	for (vec3 v : vertex_data)
	{
		raw_v_data[k++] = v.x;
		raw_v_data[k++] = v.y;
		raw_v_data[k++] = v.z;
	}
	k = 0;
	raw_i_data = new int[vertex_face.size() * 3];
	for (face f : vertex_face)
	{
		raw_i_data[k++] = f.indices[0] - 1;
		raw_i_data[k++] = f.indices[1] - 1;
		raw_i_data[k++] = f.indices[2] - 1;
	}

	raw_n_data = new float[vertex_data.size() * 3];
	raw_vt_data = new float[vertex_data.size() * 2];

	for (int j = 0; j < vertex_face.size(); j++)
	{
		face f = vertex_face[j];
		face vt_coords = texcoords_face[j];
		face vn_coords = normal_face[j];
		for (int i = 0; i < 3; i++)
		{
			vec3 to_put_tex;
			if (texcoords_data.size() == 0)
			{
				to_put_tex.x = 0;
				to_put_tex.y = 0;
				to_put_tex.z = 0;
			}
			else
				to_put_tex = texcoords_data[vt_coords.indices[i] - 1];
			// Currently only accepts two values i.e. 2D textures

			raw_vt_data[2 * (f.indices[i] - 1)] = to_put_tex.x;
			raw_vt_data[2 * (f.indices[i] - 1) + 1] = to_put_tex.y;


			if ( (vn_coords.indices[i] - 1) > normal_data.size())
			{
				std::cout << "Index: " << (vn_coords.indices[i] - 1) << std::endl
					<< "Capacity: "<< normal_data.size() << std::endl;
				break;
			}
			vec3 to_put_norm = normal_data[vn_coords.indices[i] - 1];
			//if ( ( (3 * (f.indices[i] - 1) ) + 2) > normal_data.size() * 3)
			//{
			//	std::cout << "Normal fill" << file_path << std::endl;
			//	continue;
			//}
			raw_n_data[3 * (f.indices[i] - 1)] = to_put_norm.x;
			raw_n_data[3 * (f.indices[i] - 1) + 1] = to_put_norm.y;
			raw_n_data[3 * (f.indices[i] - 1) + 2] = to_put_norm.z;
		}
	}
}

void ObjFileReader::tokenize(std::string& to_break, std::vector<std::string>& tokens, const char* delimiter)
{
	std::stringstream ss(to_break);
	std::string item;
	while(getline(ss, item, *delimiter)) {
		if (strlen(item.c_str()) > 0)
			tokens.push_back(item);
	}
}

void ObjFileReader::bufferMemory()
{
	int v_size = vertex_data.size() * 4;
	ShaderParameter pos = ShaderParameter("position", v_size * 3, 3, raw_v_data, true);
	ShaderParameter norms = ShaderParameter("normal", v_size * 3, 3, raw_n_data, true);
	ShaderParameter texcoords = ShaderParameter("texcoords", v_size * 2, 2, raw_vt_data, true);
	buffers = CudaGLBufferObj();

	buffers.initIndexBuffer(raw_i_data, vertex_face.size() * 3 * 4);
	buffers.addBuffer(pos);
	buffers.addBuffer(norms);
	buffers.addBuffer(texcoords);
}

CudaGLBufferObj* ObjFileReader::getBuffers()
{
	std::string s = buffers.getType();
	return &buffers;
}

ObjFileReader::~ObjFileReader(void)
{
}
