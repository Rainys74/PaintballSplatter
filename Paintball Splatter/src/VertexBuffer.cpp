#include "VertexBuffer.h"
#include "OpenGLDebugHandler.h"

static unsigned int bufferInstanceCount = 0;

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int usage)
	: m_RendererID(bufferInstanceCount), m_VertexArray(0), m_LayoutItemCount(0), m_BufferUsage(usage)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, m_BufferUsage);
	bufferInstanceCount++;
}

VertexBuffer::VertexBuffer(unsigned int usage)
	: m_RendererID(bufferInstanceCount), m_VertexArray(0), m_LayoutItemCount(0), m_BufferUsage(usage)
{
	glGenBuffers(1, &m_RendererID);
	bufferInstanceCount++;
}

VertexBuffer::~VertexBuffer()
{
	Delete();
}

void VertexBuffer::FinalizeLayout()
{
	// item = x = (layout = x)
	// glVertexAttribArray(layout id example: position 0 texture coordinate 1 etc., amount of numbers of floats or something per item, GL_FALSE, stride = bytes of data for a single vertex, (void*)starting position for that item, so layout 0 position = 0, layout 1 texture coordinates = [3 because 3 floats for positions * sizeof(float)])
	for (uint8_t i = 0; i < m_LayoutItemCount; i++)
	{
		Bind();
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, m_Elements[i].dataCount, m_Elements[i].dataType, m_Elements[i].normalized, m_Elements[i].bytesPerVertex, m_Elements[i].bytesPerItemStart); // offsetof(position, "my second item in Vertex class")
	}
}

void VertexBuffer::PushVertex(Vertex& vertex)
{
	m_Vertices.push_back(vertex.position.x);
	m_Vertices.push_back(vertex.position.y);
	m_Vertices.push_back(vertex.position.z);
}

void VertexBuffer::FinalizeVertices()
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), m_Vertices.data(), m_BufferUsage);
}

void VertexBuffer::Bind() const
{
	GLBindCache& cache = GLBindCache::GetInstance(BufferType::VertexBuffer);

	//if (cache.BufferBound(m_RendererID))
		//return;

	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	cache.BufferCache(m_RendererID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
	glDeleteBuffers(1, &m_RendererID);
	bufferInstanceCount--;
}
