#include "FrameBufferTexture.h"

std::stack<FrameBufferTexture*> FrameBufferTexture::fbo_stack = std::stack<FrameBufferTexture*>();

FrameBufferTexture::FrameBufferTexture(int l, int b, int w, int h) :
    left(l),
    bottom(b),
    width(w),
    height(h)
{
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &Texture::maximum_texture_units);
    texture_num = GL_TEXTURE0 + texture_count;

    tex_width = w;
    tex_height = h;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(texture_num);
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    texture_count++;
    texture_count %= Texture::maximum_texture_units;

    //Generate and attach framebuffer and renderbuffer
    glGenFramebuffers(1, &fbo_handle);
    glGenRenderbuffers(1, &rbo_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_handle);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
        rbo_handle);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << ("FBO : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
    else
        std::cout << "FrameBuffer : COMPLETE\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBufferTexture::addMesh(TriangleMesh to_add)
{
    render_objects.push_back(to_add);
}

FrameBufferTexture::FrameBufferTexture(void)
{
}


FrameBufferTexture::~FrameBufferTexture(void)
{
}

void FrameBufferTexture::bindFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_handle);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << ("FBO : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
}

void FrameBufferTexture::unbindFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (fbo_stack.size() > 0)
    {
        fbo_stack.pop();
        if(fbo_stack.size() > 0)
        {
            FrameBufferTexture* fbo_tex = fbo_stack.top();
            fbo_tex->bindFrame();
        }
    }
    else
        std::cout << "No frame bound for frame unbind" << std::cout;
}



void FrameBufferTexture::bind()
{
    fbo_stack.push(this);
    bindFrame();
    GLint* saved_vp = new GLint[4];
    glGetIntegerv(GL_VIEWPORT, saved_vp);
    glViewport(left, bottom, width, height);

    glClearColor(1.0, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.16, 0.6, 0.6, 1.0);
    for (int i = 0; i < render_objects.size(); i++)
        render_objects[i].draw();

    unbindFrame();
    
    glViewport(saved_vp[0], saved_vp[1], saved_vp[2], saved_vp[3]);

    delete[] saved_vp;

    int tex_handle = WorldState::getCurrentShader()->getUniform("texture");
    if (tex_handle == -1)
    {
        std::cout << "No texture named 'texture' in the current shader" << std::endl;
        return;
    }
    glActiveTexture(texture_num);
    glBindTexture(GL_TEXTURE_2D, handle);
    glUniform1i(tex_handle, texture_num - GL_TEXTURE0);
}

void FrameBufferTexture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

FrameBufferTexture& FrameBufferTexture::operator=(const FrameBufferTexture& rhs)
{
    fbo_handle = rhs.fbo_handle;
    rbo_handle = rhs.rbo_handle;
    left = rhs.left;
    bottom = rhs.bottom;
    width = rhs.width;
    height = rhs.height;
    handle = rhs.handle;
    texture_num = rhs.texture_num;

    return *this;
}

FrameBufferTexture::FrameBufferTexture(const FrameBufferTexture& copy)
{
    fbo_handle = copy.fbo_handle;
    rbo_handle = copy.rbo_handle;
    left = copy.left;
    bottom = copy.bottom;
    width = copy.width;
    height = copy.height;
    handle = copy.handle;
    texture_num = copy.texture_num;
}