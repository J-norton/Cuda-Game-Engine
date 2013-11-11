#pragma once
#include <stack>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Shader;
class BufferManager;
class Texture;
class FrameBufferTexture;
class UniformBlock;
class ObjectState;

class WorldState
{
private:

	static std::stack<Shader*> shader_stack;
    static std::stack<BufferManager*> buffer_stack;
    static std::stack<Texture*> texture_stack;
    static std::stack<FrameBufferTexture*> framebuffer_stack;
    static std::stack<UniformBlock*> uniformblock_stack;
    static std::stack<ObjectState*> object_stack;

public:
    static void pushShader(Shader*);
    static void pushBuffer(BufferManager*);
    static void pushTexture(Texture*);
    static void pushFramebuffer(FrameBufferTexture*);
    static void pushUniformBlock(UniformBlock*);
    static void pushObject(ObjectState* object);

    static Shader* popShader(Shader* previous);
    static BufferManager* popBuffer(BufferManager* previous);
    static Texture* popTexture(Texture* previous);
    static FrameBufferTexture* popFramebuffer(FrameBufferTexture* previous);
    static UniformBlock* popUniformBlock(UniformBlock* previous);
    static ObjectState* popObject(ObjectState* object);

	static Shader* getCurrentShader();
	static BufferManager* getCurrentBuffer();
	static Texture* getCurrentTexture();
	static FrameBufferTexture* getCurrentFramebuffer();
    static UniformBlock* getCurrentUniformBlock();
    static ObjectState* getCurrentObject();

	WorldState(void);
	~WorldState(void);
};
