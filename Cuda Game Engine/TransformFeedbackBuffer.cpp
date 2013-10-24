#include "TransformFeedbackBuffer.h"


TransformFeedbackBuffer::TransformFeedbackBuffer(std::string vs_path) :
    vs_filepath(vs_path)
{
    std::string dir = "../Test Files/Shader Programs/";
    vs_filepath = dir.append(vs_path);
}


TransformFeedbackBuffer::~TransformFeedbackBuffer(void)
{
}

void TransformFeedbackBuffer::bind()
{

    GLint frag_compiled, compile_worked, linked;

    std::ifstream src_file = std::ifstream(vs_filepath);
    std::string src_prog(std::istreambuf_iterator<char>(src_file), (std::istreambuf_iterator<char>()) );
    const char* cstr_source = src_prog.c_str();

    program = glCreateProgram();
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &cstr_source, nullptr);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_worked);
    if (compile_worked == 0)
    {
        //std::cout << "Error Compiling GLSL code" << std::endl;
        GLint length;
        GLchar* error_log;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &length);

        error_log = new GLchar[length];
        glGetShaderInfoLog(vertex_shader, length, &length, error_log);
        std::cout << error_log;
        std::string arg = std::string(error_log);
        delete[] error_log;
    }

    glAttachShader(program, vertex_shader);

    for (ShaderParameter param : parameters)
    {
        std::string trans_out = param.name;
        const char* dummy = trans_out.c_str();
        glTransformFeedbackVaryings(WorldState::getCurrentShader()->getProgHandle(), 1, &dummy, GL_SEPARATE_ATTRIBS);
        if (glGetError() != GL_NO_ERROR)
            std::cout << "Transform feedback error" << std::endl;
    }

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked)
    {
        glUseProgram(program);
    }
    else
    {
        //std::cout << "Error linking GLSL Program" << std::endl;
        GLint length;
        GLchar* error_log;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length < 1)
            length = 1024;
        error_log = new GLchar[length];

        glGetShaderInfoLog(program, length, &length, error_log);
        std::cout << error_log;
        std::string arg = std::string(error_log);
        delete[] error_log;
    }
    bound = true;

    glGenQueries(1, &query);
    glGenBuffers(1, &buffer_handle);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer_handle);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 10, nullptr, GL_STATIC_READ);

    glGenTransformFeedbacks(1, &TF_handle);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TF_handle);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer_handle); 
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
    glBeginTransformFeedback(GL_TRIANGLES);
    //TODO: PRODUCES GL_ERROR HERE=================================================================
    //TODO: PRODUCES GL_ERROR HERE=================================================================
    //TODO: PRODUCES GL_ERROR HERE=================================================================
    //TODO: PRODUCES GL_ERROR HERE=================================================================
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        std::cout << err << std::endl << "GL Error transform feedback" << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, 5);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
    glEndTransformFeedback();
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
    GLint count;
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &count); 
    if (glGetError() != GL_NO_ERROR)
        std::cout << "GL Error transform feedback" << std::endl;
}

void TransformFeedbackBuffer::unbind()
{

}

void TransformFeedbackBuffer::addInput(TriangleMesh vertex_input)
{
    vertex_inputs.push_back(vertex_input);
}

void TransformFeedbackBuffer::addOutput(std::string output)
{
    outputs.push_back(output);
}