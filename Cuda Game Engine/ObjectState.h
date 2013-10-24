#pragma once
#include "Shader.h"
#include "BufferManager.h"
#include "UniformBlock.h"
#include "Texture.h"
#include "WorldState.h"


class ObjectState
{
private:
    Shader* program;
    BufferManager* buffer_manager;
    std::vector<std::pair<std::string, int> > uniform_pairs;
    //std::vector<TransformFeedbackBuffer> transform_buffers;
    std::vector<BufferManager*> transform_buffers;
    Texture* texture;
    Texture frame_texture;
    int num_vertices;
    Shader* prev_shader;
    Texture* prev_texture;
    BufferManager* prev_buffer;
    //Tracks the total amount of data buffered to GL_UNIFORM_BUFFER
    static int uniform_buffer_posn;
public:
    void addTransformBuffer(BufferManager* buffer);
    //Object state is responsible for setting block 
    void addUniformPair(std::pair<std::string, int> shader_block_map);
    void setTexture(Texture* new_texture);
    void setTexture(Texture new_texture);
    int getNumVertices();
    void bind();
    void unbind();
    std::vector<UniformBlock>& getUniforms();
    ObjectState(const ObjectState& copy);
    ObjectState& operator=(const ObjectState& rhs);
    ObjectState(BufferManager* memory, Shader* program, Texture* texture);
    ObjectState(void);
    ~ObjectState(void);
};

