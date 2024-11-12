#pragma once
#if 0
#include <GL/glew.h>

#include <vector>

#include "logger.h"
#include "EngineMath.h"

#include "Buffer.h"

struct Vertex // vertex data
{
	Vertex() 
		: position(0, 0, 0) {}
	Vertex(float x, float y, float z) 
		: position(x, y, z) {}

	Math::Vector3 position;
};

class VertexBufferLayout
{
public:
	unsigned int itemID;			// 0 - position, 1 - texture coordinates
	unsigned int dataCount;			// 3 for position, 2 for texture coordinates
	unsigned int bytesPerVertex;	// 3+2=5*sizeof(float) = the amount of bytes of data for a single vertex
	void* bytesPerItemStart;		// starting position for the item id, 0 - 3 * sizeof(float), 1 - 2 * sizeof(float) or just use offsetof(firstItem, secondItem)
	unsigned char normalized;		// GL_FALSE
	int dataType;					// GL_FLOAT, GL_UNSIGNED_INT
};

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	VertexBuffer(unsigned int usage);
	~VertexBuffer();

	unsigned int GetItemID() { return m_LayoutItemCount; }

	template<typename T>
	void PushLayout(unsigned int itemID, unsigned int dataCount, unsigned int bytesPerVertex, void* bytesPerItemStart, unsigned char normalized = GL_FALSE) // LayoutPush
	{
		PS_ASSERT(false, "Unrecognized vertex buffer data type."); //std::runtime_error(false); //static_assert(false);
	}

	template<>
	void PushLayout<float>(unsigned int itemID, unsigned int dataCount, unsigned int bytesPerVertex, void* bytesPerItemStart, unsigned char normalized)
	{
		m_Elements.push_back(VertexBufferLayout{itemID, dataCount, bytesPerVertex, bytesPerItemStart, normalized, GL_FLOAT});
		m_LayoutItemCount++;
	}

	void FinalizeLayout(); // set layout
	void PushVertex(Vertex& vertex);
	void FinalizeVertices();

	void Bind() const;
	static void Unbind();
	void Delete();
private:
	unsigned int m_RendererID;
	unsigned int m_VertexArray;
	unsigned int m_BufferUsage;

	unsigned int m_LayoutItemCount;
	std::vector<VertexBufferLayout> m_Elements;
	std::vector<float> m_Vertices;
};
#else
#include "DX11Includes.h"

#include <vector>

#include "logger.h"
#include "EngineMath.h"

#include "Buffer.h"

struct Vertex // vertex data
{
	Vertex()
		: position(0, 0, 0) {}
	Vertex(float x, float y, float z)
		: position(x, y, z) {}

	Math::Vector3 position;
};

class VertexBufferLayout
{
public:
	unsigned int itemID;			// 0 - position, 1 - texture coordinates
	unsigned int dataCount;			// 3 for position, 2 for texture coordinates
	unsigned int bytesPerVertex;	// 3+2=5*sizeof(float) = the amount of bytes of data for a single vertex
	void* bytesPerItemStart;		// starting position for the item id, 0 - 3 * sizeof(float), 1 - 2 * sizeof(float) or just use offsetof(firstItem, secondItem)
	unsigned char normalized;		// GL_FALSE
	int dataType;					// GL_FLOAT, GL_UNSIGNED_INT
};

//#define GFX_FLOAT 0x1406 // GL_FLOAT
//#define GFX_UNSIGNED_INT 0x1405 // GL_UNSIGNED_INT
//
//#define GFX_FALSE 0 // GL_FALSE

#define GFX_FLOAT 0x1406
#define GFX_UNSIGNED_INT 0x1405

#define GFX_FALSE FALSE

#define GFX_STATIC_DRAW (unsigned int)D3D11_USAGE_DEFAULT
#define GFX_DYNAMIC_DRAW (unsigned int)D3D11_USAGE_DYNAMIC

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	VertexBuffer(unsigned int usage);
	~VertexBuffer();

	unsigned int GetItemID() { return m_LayoutItemCount; }

	template<typename T>
	void PushLayout(unsigned int itemID, unsigned int dataCount, unsigned int bytesPerVertex, void* bytesPerItemStart, unsigned char normalized = GFX_FALSE) // LayoutPush
	{
		PS_ASSERT(false, "Unrecognized vertex buffer data type."); //std::runtime_error(false); //static_assert(false);
	}

	template<>
	void PushLayout<float>(unsigned int itemID, unsigned int dataCount, unsigned int bytesPerVertex, void* bytesPerItemStart, unsigned char normalized)
	{
		m_Elements.push_back(VertexBufferLayout{ itemID, dataCount, bytesPerVertex, bytesPerItemStart, normalized, GFX_FLOAT });
		m_LayoutItemCount++;
	}

	void FinalizeLayout(); // set layout
	void PushVertex(Vertex& vertex);
	void FinalizeVertices();

	void Bind() const;
	static void Unbind();
	void Delete();
private:
	WRL::ComPtr<ID3D11Buffer> m_RendererPtr;
	unsigned int m_VertexArray;
	unsigned int m_BufferUsage;

	unsigned int m_LayoutItemCount;
	std::vector<VertexBufferLayout> m_Elements;
	std::vector<float> m_Vertices;
};
#endif