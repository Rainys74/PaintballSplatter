#include "IndexBuffer.h"
#include "logger.h"
#include <GL/glew.h>

#include "Buffer.h"

static unsigned int bufferInstanceCount = 0;

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_RendererID(bufferInstanceCount), m_Count(count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	bufferInstanceCount++;
}

IndexBuffer::~IndexBuffer()
{
	Delete();
}

void IndexBuffer::Bind() const
{
	GLBindCache& cache = GLBindCache::GetInstance(BufferType::IndexBuffer);

	if (cache.BufferBound(m_RendererID))
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // bind index buffer
	cache.BufferCache(m_RendererID);
}

void IndexBuffer::Delete()
{
	glDeleteBuffers(1, &m_RendererID);
	bufferInstanceCount--;
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind index buffer
}