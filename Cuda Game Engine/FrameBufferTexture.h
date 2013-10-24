#pragma once
#include <iostream>
#include "WorldState.h"
#include "Texture.h"
#include <vector>
#include "TriangleMesh.h"
#include "TransformFeedbackBuffer.h"

class FrameBufferTexture :
    public Texture
{
private:
    //Tracks other framebuffers in order to nest framebuffer textures
    static std::stack<FrameBufferTexture*> fbo_stack;
	//Handles to the framebuffer and renderbuffer object
	unsigned int fbo_handle;
	unsigned int rbo_handle;
	int left;
	int bottom;
	int width;
	int height;
    std::vector<TriangleMesh> render_objects;
    FrameBufferTexture* previous_fbo;
public:
    void addMesh(TriangleMesh to_add);
    void bindFrame();
    void unbindFrame();
    void bind();
    void unbind();
	FrameBufferTexture(int left, int bottom, int width, int height);
	FrameBufferTexture(void);
	FrameBufferTexture(const FrameBufferTexture& copy);
	FrameBufferTexture& operator=(const FrameBufferTexture& rhs);
	~FrameBufferTexture(void);
};

