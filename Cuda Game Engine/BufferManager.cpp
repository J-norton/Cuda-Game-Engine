#include "BufferManager.h"


BufferManager::BufferManager(void) :
    data(nullptr),
    bound(false),
    has_indices(false),
    index_handle(0),
    num_vertices(0),
    parameters(std::vector<ShaderParameter>()),
    stride(0)
{
}


BufferManager::~BufferManager(void)
{
}

void BufferManager::addBuffer(ShaderParameter param)
{
    parameters.push_back(param);
}

std::string BufferManager::getType()
{
	return "GL VBO";
}

void BufferManager::bind()
{
	//If the parameter size is zero, there is no data to bind to the GL
    if (parameters.size() == 0) {
        std::cout << "Empty buffers for shader object: \n";
        return;
    }

    //WorldState::current_buffer = this;
    //WorldState::pushBuffer(this);

    if (!bound) {
        initBuffers();
        bound = true;
    }

    glBindBuffer(GL_ARRAY_BUFFER, interleave_handle);
    if (glGetError() != 0)
        std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";
    if (has_indices)
        bindIndices();

    GLvoid* offset = 0;
    int address = 0;
    for (ShaderParameter parameter : parameters)
    {
        //GLuint shader_num = WorldState::current_shader->getAttrib(parameter.name);
        GLuint shader_num = WorldState::getCurrentShader()->getAttrib(parameter.name);
        if (shader_num == -1)
        {
            address += parameter.cardinality * 4;
            offset = (GLvoid*) address;
            continue;
        }
        glVertexAttribPointer(shader_num, parameter.cardinality, GL_FLOAT, false, stride, offset);
        if (glGetError() != 0)
            std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";

        address += parameter.cardinality * 4;
        offset = (GLvoid*) address;
        glEnableVertexAttribArray(shader_num);
        if (glGetError() != 0)
            std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (glGetError() != 0)
        std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";
}

void BufferManager::initBuffers()
{
    //First determine the size of the entire buffer
    int buffer_size = 0;
    //Represents the number of bytes between the ith and i + 1th item in the buffer
    stride = 0;
    for (ShaderParameter p : parameters)
    {
        buffer_size += p.size;
        stride += p.cardinality * 4;
        p.shader_handle = WorldState::getCurrentShader()->getAttrib(p.name);
    }

    int vertex_count = buffer_size / stride;
    float* interleaved_data = new float[buffer_size / 4];
    int k = 0;

    //The following assumes that all parameters have the same number of elements
    for (int i = 0; i < vertex_count; i++)
    {
        for (ShaderParameter p : parameters)
        {
            //The index into the array that will
            int offset = p.cardinality * i;
            for (int j = 0; j < p.cardinality; j++)
                interleaved_data[k++] = (p.client_data[offset + j]);
        }
    }
    GLuint* buffer_return = new GLuint[1];
    //Finally generate and bind the buffer
    glGenBuffers(1, buffer_return);
    interleave_handle = buffer_return[0];
    glBindBuffer(GL_ARRAY_BUFFER, interleave_handle);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, &interleaved_data[0], GL_STATIC_DRAW);
    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] buffer_return;
    delete[] interleaved_data;
    //Delete all individual client-side arrays
    for (ShaderParameter p : parameters)
    {
        if (p.delete_on_buffer)
            delete[] p.client_data;
    }
    bound = true;
}

void BufferManager::unbind()
{
    for (ShaderParameter param : parameters)
    {
        GLuint parameter_id = WorldState::getCurrentShader()->getAttrib(param.name);
        if (parameter_id == -1)
            continue;
        glDisableVertexAttribArray(parameter_id);
    }
    if (has_indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BufferManager::bindIndices()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_handle);
}

int BufferManager::getNumVertices()
{
    return num_vertices;
}

void BufferManager::initIndexBuffer(int* indices, int n)
{
    has_indices = true;
    unsigned int* temp = new unsigned int[1];
    num_vertices = n / 4;

    glGenBuffers(1, temp);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp[0]);
    index_handle = temp[0];
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n, indices,
        GL_STATIC_DRAW);
    // Unbind buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Now that the memory is buffered, delete the client-side ints
    delete[] indices;
}