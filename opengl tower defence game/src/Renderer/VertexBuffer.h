#pragma once

#include "GL/glew.h"

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int type = GL_DYNAMIC_DRAW);
	VertexBuffer();
	~VertexBuffer();

	void Set(const void* data, unsigned int size, unsigned int type = GL_DYNAMIC_DRAW);
	void Bind() const;
	void Unbind() const;
	void AppendToBuffer() const;
};
