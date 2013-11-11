#pragma once
#include "Shader.h"
#include "BufferManager.h"
#include "UniformBlock.h"
#include "Texture.h"
#include "WorldState.h"


class ObjectState
{
private:
    std::vector<std::pair<std::string, int> > uniform_pairs;
    std::vector<BufferManager*> transform_buffers;
    Texture texture;
    Texture frame_texture;
    int num_vertices;

    //Tracks the total amount of data buffered to GL_UNIFORM_BUFFER
    static int uniform_buffer_posn;
public:
    void addTransformBuffer(BufferManager* buffer);
    //Object state is responsible for setting block 
    void addUniformPair(std::pair<std::string, int> shader_block_map);
    void setTexture(Texture new_texture);
    void bind();
    void unbind();
    std::vector<UniformBlock>& getUniforms();
    ObjectState(const ObjectState& copy);
    ObjectState& operator=(const ObjectState& rhs);
    ObjectState(Texture texture);
    ObjectState(void);
    ~ObjectState(void);
};

