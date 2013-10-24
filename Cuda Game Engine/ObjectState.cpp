#include "ObjectState.h"

int ObjectState::uniform_buffer_posn = 0;

ObjectState::ObjectState(BufferManager* memory, Shader* prog, Texture* tex) :
    buffer_manager(memory),
    program(prog),
    num_vertices(memory->getNumVertices()),
    texture(tex),
    prev_shader(nullptr),
    prev_texture(nullptr),
    uniform_pairs(std::vector<std::pair<std::string, int> >() )
{
}

ObjectState::ObjectState(void) 
{
}


ObjectState::~ObjectState(void)
{
}

void ObjectState::bind()
{
    WorldState::pushObject(this);
    try
    {
        if (program != nullptr)
        {
            program->bind();
            WorldState::pushShader(program);
            for (int i = 0; i < uniform_pairs.size(); i++)
            {
				std::pair<std::string, int> pair = uniform_pairs[i];
                int uid = program->getUniformBlockIndex(pair.first);
				if (uid < 0)
				{
					uniform_pairs.erase(uniform_pairs.begin() + i);
					continue;
				}
                int prog_id = program->getProgHandle();
                glUniformBlockBinding(prog_id, uid, pair.second);
                if (glGetError() != GL_NO_ERROR)
                    std::cout << "Uniform Block buffer error" << std::endl;
            }
        }
    }
    catch (ScenarioException e)
    {
        throw e;
    }

    for (BufferManager* buffer : transform_buffers)
    {
        buffer->bind();
    }
    if (buffer_manager != nullptr)
    {
        buffer_manager->bind();
        WorldState::pushBuffer(buffer_manager);
    }
    if (texture != nullptr)
    {
        texture->bind();
        WorldState::pushTexture(texture);
    }
}

void ObjectState::unbind()
{
    ObjectState* previous_obj = WorldState::popObject(this);

    if (texture != nullptr)
    {
        texture->unbind();
        prev_texture = WorldState::popTexture(texture);
    }
    if (buffer_manager != nullptr)
    {
        buffer_manager->unbind();
        prev_buffer = WorldState::popBuffer(buffer_manager);
    }
    if (program != nullptr)
    {
        program->unbind();
        prev_shader = WorldState::popShader(program);
    }

    if (prev_shader != nullptr)
        prev_shader->bind();

    if (previous_obj != nullptr)
    {
        for (std::pair<std::string, int> pair  : previous_obj->uniform_pairs)
        {
            int uid = prev_shader->getUniformBlockIndex(pair.first);
            int prog_id = prev_shader->getProgHandle();
            glUniformBlockBinding(prog_id, uid, pair.second);
            if (glGetError() != GL_NO_ERROR)
                std::cout << "Uniform Block buffer error" << std::endl;
        }
    }

    if (prev_buffer != nullptr)
        prev_buffer->bind();

    if (prev_texture != nullptr)
        prev_texture->bind();
}

int ObjectState::getNumVertices()
{
    return buffer_manager->getNumVertices();
}


ObjectState::ObjectState(const ObjectState& copy) :
    program(copy.program),
    buffer_manager(copy.buffer_manager),
    transform_buffers(copy.transform_buffers),
    uniform_pairs(copy.uniform_pairs),
    texture(copy.texture),
    num_vertices(copy.num_vertices),
    prev_shader(nullptr),
    prev_texture(nullptr)
{
}

ObjectState& ObjectState::operator=(const ObjectState& rhs)
{
    program = rhs.program;
    buffer_manager = rhs.buffer_manager;
    transform_buffers = rhs.transform_buffers;
    uniform_pairs = rhs.uniform_pairs;
    texture = rhs.texture;
    num_vertices = rhs.num_vertices;
    prev_shader = nullptr;
    prev_texture = nullptr;
    return *this;
}

void ObjectState::setTexture(Texture* new_texture)
{
    texture = new_texture;
}

void ObjectState::setTexture(Texture new_texture)
{
    frame_texture = new_texture;
    texture = &frame_texture;
}


void ObjectState::addUniformPair(std::pair<std::string, int> to_add)
{
    uniform_pairs.push_back(to_add);
}

void ObjectState::addTransformBuffer(BufferManager* feedback_buffer)
{
    transform_buffers.push_back(feedback_buffer);
}