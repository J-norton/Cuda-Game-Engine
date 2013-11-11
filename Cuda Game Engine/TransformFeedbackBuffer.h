#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "BufferManager.h"
#include "TriangleMesh.h"

class TransformFeedbackBuffer :
    public BufferManager
{
private:
    std::string vs_filepath;
    std::vector<std::string> outputs;
    std::vector<TriangleMesh> vertex_inputs;
    unsigned int program;
    unsigned int vertex_shader;
    unsigned int query;
    unsigned int buffer_handle;
    unsigned int TF_handle;
public:
    void addOutput(std::string out_name);
    void addInput(TriangleMesh transform_input);
    virtual void bind();
    virtual void unbind();
    TransformFeedbackBuffer(std::string vs_shader_path);
    TransformFeedbackBuffer(void);
    ~TransformFeedbackBuffer(void);
};

