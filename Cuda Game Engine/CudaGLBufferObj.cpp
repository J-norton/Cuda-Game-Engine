//#include "CudaGLBufferObj.h"
//
//
//CudaGLBufferObj::CudaGLBufferObj(void)
//{
//}
//
//
//CudaGLBufferObj::~CudaGLBufferObj(void)
//{
//}
//
//std::string CudaGLBufferObj::getType()
//{
//	return "CUDA + GL VBO";
//}
//
//void CudaGLBufferObj::bind()
//{
//	//If the parameter size is zero, there is no data to bind to the GL
//    if (parameters.size() == 0) {
//        std::cout << "Empty buffers for shader object: \n";
//        return;
//    }
//
//    if (!bound) {
//        initBuffers();
//        bound = true;
//    }
//
//	glBindVertexArray(vao_handle);
//    if (has_indices)
//        bindIndices();
//}
//
//void CudaGLBufferObj::unbind()
//{
//	glBindVertexArray(0);
//}
//
//void CudaGLBufferObj::initBuffers()
//{
//    //First determine the size of the entire buffer
//    int buffer_size = 0;
//    //Represents the number of bytes between the ith and i + 1th item in the buffer
//    stride = 0;
//    for (ShaderParameter p : parameters)
//    {
//        buffer_size += p.size;
//        stride += p.cardinality * 4;
//        p.shader_handle = WorldState::getCurrentShader()->getAttrib(p.name);
//    }
//
//    int vertex_count = buffer_size / stride;
//    float* interleaved_data = new float[buffer_size / 4];
//    int k = 0;
//
//    //The following assumes that all parameters have the same number of elements
//    for (int i = 0; i < vertex_count; i++)
//    {
//        for (ShaderParameter p : parameters)
//        {
//            //The index into the array that will
//            int offset = p.cardinality * i;
//            for (int j = 0; j < p.cardinality; j++)
//                interleaved_data[k++] = (p.client_data[offset + j]);
//        }
//    }
//	//Create a Vertex Array Object so that we can bind the VBOs to it
//	glGenVertexArrays(1, &vao_handle);
//	glBindVertexArray(vao_handle);
//    GLuint* buffer_return = new GLuint[1];
//    //Finally generate and bind the buffer
//    glGenBuffers(1, buffer_return);
//    interleave_handle = buffer_return[0];
//    glBindBuffer(GL_ARRAY_BUFFER, interleave_handle);
//    glBufferData(GL_ARRAY_BUFFER, buffer_size, &interleaved_data[0], GL_STATIC_DRAW);
//	//Describe the layout of the data to the GL
//    GLvoid* offset = 0;
//    int address = 0;
//    for (ShaderParameter parameter : parameters)
//    {
//        //GLuint shader_num = WorldState::current_shader->getAttrib(parameter.name);
//        GLuint shader_num = WorldState::getCurrentShader()->getAttrib(parameter.name);
//        if (shader_num == -1)
//        {
//            address += parameter.cardinality * 4;
//            offset = (GLvoid*) address;
//            continue;
//        }
//        glVertexAttribPointer(shader_num, parameter.cardinality, GL_FLOAT, false, stride, offset);
//        if (glGetError() != 0)
//            std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";
//
//        address += parameter.cardinality * 4;
//        offset = (GLvoid*) address;
//        glEnableVertexAttribArray(shader_num);
//        if (glGetError() != 0)
//            std::cout << glGetError() << std::endl << "Illegal arguments set for framebuffer texture";
//    }
//    //Unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	cudaError_t err = cudaGraphicsGLRegisterBuffer(&cuda_vbo, interleave_handle, cudaGraphicsMapFlagsWriteDiscard);
//	if (err != 0)
//		std::cout << "Could not map GL Buffer to CUDA" << std::endl;
//
//    delete[] buffer_return;
//    delete[] interleaved_data;
//    //Delete all individual client-side arrays
//    for (ShaderParameter p : parameters)
//    {
//        if (p.delete_on_buffer)
//            delete[] p.client_data;
//    }
//    bound = true;
//}
