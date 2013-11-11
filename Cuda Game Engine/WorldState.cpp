#include "WorldState.h"

std::stack<Shader*> WorldState::shader_stack = std::stack<Shader*>();
std::stack<BufferManager*> WorldState::buffer_stack = std::stack<BufferManager*>();
std::stack<Texture*> WorldState::texture_stack = std::stack<Texture*>();
std::stack<FrameBufferTexture*> WorldState::framebuffer_stack = std::stack<FrameBufferTexture*>();
std::stack<UniformBlock*> WorldState::uniformblock_stack = std::stack<UniformBlock*>();
std::stack<ObjectState*> WorldState::object_stack = std::stack<ObjectState*>();

WorldState::WorldState(void)
{
}


WorldState::~WorldState(void)
{
}

void WorldState::pushShader(Shader* top)
{
    shader_stack.push(top);
}
void WorldState::pushBuffer(BufferManager* top)
{
    buffer_stack.push(top);
}
void WorldState::pushTexture(Texture* top)
{
    texture_stack.push(top);
}
void WorldState::pushFramebuffer(FrameBufferTexture* top)
{
    framebuffer_stack.push(top);
}

void WorldState::pushObject(ObjectState* top)
{
    object_stack.push(top);
}

Shader* WorldState::popShader(Shader* previous)
{
    if (shader_stack.size() == 0)
    {
        std::cout << "Caught attempt to pop shader without any previously bound shaders." << std::endl;
        return nullptr;
    }
    if (shader_stack.top() != previous)
        std::cout << "Incorrect nesting of Shader pushes and pops." << std::endl
        << "A different instance of Shader is unbinding another Shader object" << std::endl;
    shader_stack.pop();
    if (shader_stack.size() > 0)
        return shader_stack.top();
    else
        return nullptr;
}
BufferManager* WorldState::popBuffer(BufferManager* previous)
{
    if (buffer_stack.top() != previous)
        std::cout << "Incorrect nesting of BufferManager pushes and pops." << std::endl
        << "A different instance of BufferManager is unbinding another BufferManager object" << std::endl;
    buffer_stack.pop();
    if (buffer_stack.size() > 0)
        return buffer_stack.top();
    else
        return nullptr;
}
Texture* WorldState::popTexture(Texture* previous)
{
    if (texture_stack.top() != previous)
        std::cout << "Incorrect nesting of Texture pushes and pops." << std::endl
        << "A different instance of Texture is unbinding another Texture object" << std::endl;
    texture_stack.pop();
    if (texture_stack.size() > 0)
        return texture_stack.top();
    else
        return nullptr;
}


FrameBufferTexture* WorldState::popFramebuffer(FrameBufferTexture* previous)
{
    if (framebuffer_stack.top() != previous)
        std::cout << "Incorrect nesting of Framebuffer pushes and pops." << std::endl
        << "A different instance of FrameBufferTexture is unbinding another FBO object" << std::endl;
    framebuffer_stack.pop();
    if (framebuffer_stack.size() > 0)
        return framebuffer_stack.top();
    else
        return nullptr;
}

UniformBlock* WorldState::popUniformBlock(UniformBlock* previous)
{
    if (uniformblock_stack.top() != previous)
        std::cout << "Incorrect nesting of Framebuffer pushes and pops." << std::endl
        << "A different instance of FrameBufferTexture is unbinding another FBO object" << std::endl;
    uniformblock_stack.pop();
    if (uniformblock_stack.size() > 0)
        return uniformblock_stack.top();
    else
        return nullptr;
}

ObjectState* WorldState::popObject(ObjectState* previous)
{
    if (object_stack.size() == 0)
    {
        std::cout << "Caught attempt to pop Object without any previously bound objects." << std::endl;
        return nullptr;
    }
    if (object_stack.top() != previous)
        std::cout << "Incorrect nesting of ObjectState pushes and pops." << std::endl
        << "A different instance of ObjectState is unbinding another object" << std::endl;
    object_stack.pop();
    if (object_stack.size() > 0)
        return object_stack.top();
    else
        return nullptr;
}

Shader* WorldState::getCurrentShader()
{
    return shader_stack.top();
}
BufferManager* WorldState::getCurrentBuffer()
{
    return buffer_stack.top();
}
Texture* WorldState::getCurrentTexture()
{
    return texture_stack.top();
}
FrameBufferTexture* WorldState::getCurrentFramebuffer()
{
    return framebuffer_stack.top();
}

UniformBlock* WorldState::getCurrentUniformBlock()
{
    return uniformblock_stack.top();
}

void WorldState::pushUniformBlock(UniformBlock* top)
{
    uniformblock_stack.push(top);
}