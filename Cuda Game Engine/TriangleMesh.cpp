#include "TriangleMesh.h"


TriangleMesh::TriangleMesh(void) :
    num_instances(1),
    counter(0),
	static_object(false)
{
	vec3 zero = vec3(0, 0, 0);
	position = zero;
	scale = zero;
	vec3 initial_velocity = vec3(0, 0, 0.09);
	velocity = initial_velocity;
}

TriangleMesh::TriangleMesh(int instance_count) :
    num_instances(instance_count),
	static_object(false)
{
}

TriangleMesh::TriangleMesh(Shader prog_shader, BufferManager buffer, Texture texture_mat, bool static_mesh) :
	static_object(static_mesh),
    num_instances(1),
    counter(0),
	buffers(buffer),
	texture(texture_mat)
{
	shader = prog_shader;
}

void TriangleMesh::setPosition(vec3 new_value)
{
	position = new_value;
}

void TriangleMesh::setVelocity(vec3 new_value)
{
	velocity = new_value;
}

void TriangleMesh::setScale(vec3 new_value)
{
	scale = new_value;
}

void TriangleMesh::updateUniforms()
{
	//position = position + velocity;
	//velocity.y -= 0.001;
	//shader.setUniform("translation", position);
	//if (position.y < -10.0)
	//	velocity.y += 0.006;
	//if (position.x < -10.0)
	//	velocity.x += 0.006;
	//if (position.z < -10.0)
	//	velocity.z += 0.006;

	//if (position.x > 10.0)
	//	velocity.x -= 0.006;
	//if (position.z > 10.0)
	//	velocity.z -= 0.006;
}

TriangleMesh::~TriangleMesh(void)
{
}

void TriangleMesh::draw()
{
	WorldState::pushShader(&shader);
	shader.bind();
	buffers.bind();
	WorldState::pushBuffer(&buffers);
	updateUniforms();
	texture.bind();
	WorldState::pushTexture(&texture);
    glDrawElements(GL_TRIANGLES, buffers.getNumVertices(), GL_UNSIGNED_INT, NULL);
    GLint num;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &num);
    if (glGetError() != GL_NO_ERROR)
        std::cout << "Draw buffer state check failed" << std::endl;

    //glDrawElementsInstanced(GL_TRIANGLES, mesh_state.getNumVertices(), GL_UNSIGNED_INT, NULL, num_instances);
	texture.unbind();
	WorldState::popTexture(&texture);
	buffers.unbind();
	WorldState::popBuffer(&buffers);
	shader.unbind();
	WorldState::popShader(&shader);
}

TriangleMesh::TriangleMesh(const TriangleMesh& copy) :
    num_instances(copy.num_instances),
	position(copy.position),
	velocity(copy.velocity),
	scale(copy.scale),
	shader(copy.shader),
	static_object(copy.static_object),
	buffers(copy.buffers),
	texture(copy.texture)
{
    counter = copy.counter;
    //mesh_state = copy.mesh_state;
}

TriangleMesh& TriangleMesh::operator=(const TriangleMesh& rhs) 
{
	texture = rhs.texture;
	buffers = rhs.buffers;
	position = rhs.position;
	velocity = rhs.velocity;
	scale = rhs.scale;
    counter = rhs.counter;
    num_instances = rhs.num_instances;
    //mesh_state = rhs.mesh_state;
	static_object = rhs.static_object;
	shader = rhs.shader;

    return *this;
}

//ObjectState& TriangleMesh::getState()
//{
//	return mesh_state;
//}

void TriangleMesh::setTexture(Texture new_texture)
{
	texture = new_texture;
    //mesh_state.setTexture(new_texture);
}

