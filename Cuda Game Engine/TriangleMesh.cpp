#include "TriangleMesh.h"


TriangleMesh::TriangleMesh(void) :
    num_instances(1),
    counter(0)
{
}

TriangleMesh::TriangleMesh(ObjectState draw_state, int instance_count) :
    num_instances(instance_count)
{
    mesh_state = draw_state;
}

TriangleMesh::TriangleMesh(ObjectState draw_state) :
    num_instances(1),
    counter(0)
{
    mesh_state = draw_state;
}


TriangleMesh::~TriangleMesh(void)
{
}

void TriangleMesh::draw()
{
    mesh_state.bind();
    glDrawElements(GL_TRIANGLES, mesh_state.getNumVertices(), GL_UNSIGNED_INT, NULL);
    GLint num;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &num);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "Draw buffer state check failed" << std::endl;

    //glDrawElementsInstanced(GL_TRIANGLES, mesh_state.getNumVertices(), GL_UNSIGNED_INT, NULL, num_instances);
    mesh_state.unbind();
}

TriangleMesh::TriangleMesh(const TriangleMesh& copy) :
    num_instances(copy.num_instances)
{
    counter = copy.counter;
    mesh_state = copy.mesh_state;
}

TriangleMesh& TriangleMesh::operator=(const TriangleMesh& rhs) 
{
    counter = rhs.counter;
    num_instances = rhs.num_instances;
    mesh_state = rhs.mesh_state;
    return *this;
}

void TriangleMesh::setText(std::string message)

{
    Magick::Image error_message("256x256", "red");
    //error_message.annotate(message, Magick::Geometry(128, 128, 128, 128));
    Texture error_texture = Texture(error_message);
    mesh_state.setTexture(&error_texture);
}

void TriangleMesh::setTexture(Texture new_texture)
{
    mesh_state.setTexture(new_texture);
}

