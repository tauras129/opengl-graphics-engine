#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int type /*= GL_DYNAMIC_DRAW*/)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	std::cout << m_RendererID << " is the id of the buffer" << std::endl;
	//NOTE: data can be nullptr, size is max size of the buffer in bytes
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
