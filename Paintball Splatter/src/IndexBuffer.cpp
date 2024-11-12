#include "IndexBuffer.h"
#if 0
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
#else
#include "logger.h"
#include "Renderer.h"

#include "Buffer.h"

static unsigned int bufferInstanceCount = 0;

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_RendererPtr(0), m_Count(count)
{
	//glGenBuffers(1, &m_RendererID);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT; //(D3D11_USAGE)GFX_STATIC_DRAW;
	ibd.CPUAccessFlags = 0;
	if (ibd.Usage == D3D11_USAGE_DYNAMIC)
		ibd.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.ByteWidth = m_Count * sizeof(unsigned int);
	ibd.StructureByteStride = sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = data;

	HRESULT hr;
	DXCallHR(Renderer::GetDevice()->CreateBuffer(&ibd, &isd, &m_RendererPtr));

	// Bind index buffer
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_RendererPtr.Get(), DXGI_FORMAT_R32_UINT, 0);

	bufferInstanceCount++;
}

IndexBuffer::~IndexBuffer()
{
	Delete();
}

void IndexBuffer::Bind() const
{
	//GLBindCache& cache = GLBindCache::GetInstance(BufferType::IndexBuffer);
	//
	//if (cache.BufferBound(m_RendererID))
	//	return;
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // bind index buffer
	//cache.BufferCache(m_RendererID);

	// Bind index buffer
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_RendererPtr.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::Delete()
{
	//glDeleteBuffers(1, &m_RendererID);
	bufferInstanceCount--;
}

void IndexBuffer::Unbind()
{
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind index buffer
}
#endif