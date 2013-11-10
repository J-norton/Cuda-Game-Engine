#include "ObjectState.h"

int ObjectState::uniform_buffer_posn = 0;

ObjectState::ObjectState(Texture tex) :
	texture(tex),
	uniform_pairs(std::vector<std::pair<std::string, int> >() )
{
}

ObjectState::ObjectState(void) 
{
}


ObjectState::~ObjectState(void)
{
}

void ObjectState::bind()
{
	WorldState::pushObject(this);

	for (BufferManager* buffer : transform_buffers)
	{
		buffer->bind();
	}
	texture.bind();
	WorldState::pushTexture(&texture);
}

void ObjectState::unbind()
{
	WorldState::popObject(this);

	texture.unbind();
	WorldState::popTexture(&texture);
}


ObjectState::ObjectState(const ObjectState& copy) :
	transform_buffers(copy.transform_buffers),
	uniform_pairs(copy.uniform_pairs),
	texture(copy.texture),
	num_vertices(copy.num_vertices)
{
}

ObjectState& ObjectState::operator=(const ObjectState& rhs)
{
	transform_buffers = rhs.transform_buffers;
	uniform_pairs = rhs.uniform_pairs;
	texture = rhs.texture;
	num_vertices = rhs.num_vertices;
	return *this;
}

void ObjectState::setTexture(Texture new_texture)
{
	texture = new_texture;
}


void ObjectState::addUniformPair(std::pair<std::string, int> to_add)
{
	uniform_pairs.push_back(to_add);
}

void ObjectState::addTransformBuffer(BufferManager* feedback_buffer)
{
	transform_buffers.push_back(feedback_buffer);
}