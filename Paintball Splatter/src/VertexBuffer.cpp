#include "VertexBuffer.h"

#if 0
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
#else
#include "Renderer.h"

static unsigned int bufferInstanceCount = 0;

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int usage)
	: m_RendererPtr(0), m_VertexArray(0), m_LayoutItemCount(0), m_BufferUsage(usage)
{
	//glGenBuffers(1, &m_RendererID);
	//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//glBufferData(GL_ARRAY_BUFFER, size, data, m_BufferUsage);

	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = (D3D11_USAGE)usage;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	// NOTE: first issue: d3d11 has no vertex arrays so the layout needs to be decided here.
	// future workaround: make sure you pass in the layout data structure/descriptor when creating a vertex buffer.
	// current dumb idiotic retarted workaround: move the code into FinalizeLayout()
	D3D11_SUBRESOURCE_DATA vsd = {};
	//Renderer::GetDevice()->CreateBuffer();


	// ^^ is unneeded lmao (i'm not commenting out)

	// Copy the data into the m_Vertices vector
	m_Vertices = std::vector<float>(static_cast<const float*>(data),
		static_cast<const float*>(data) + size / sizeof(float));

	bufferInstanceCount++;
}

VertexBuffer::VertexBuffer(unsigned int usage)
	: m_RendererPtr(0), m_VertexArray(0), m_LayoutItemCount(0), m_BufferUsage(usage)
{
	//glGenBuffers(1, &m_RendererID);



	bufferInstanceCount++;
}

VertexBuffer::~VertexBuffer()
{
	Delete();
}

void VertexBuffer::FinalizeLayout()
{
	//// item = x = (layout = x)
	//// glVertexAttribArray(layout id example: position 0 texture coordinate 1 etc., amount of numbers of floats or something per item, GL_FALSE, stride = bytes of data for a single vertex, (void*)starting position for that item, so layout 0 position = 0, layout 1 texture coordinates = [3 because 3 floats for positions * sizeof(float)])
	//for (uint8_t i = 0; i < m_LayoutItemCount; i++)
	//{
	//	Bind();
	//	glEnableVertexAttribArray(i);
	//	glVertexAttribPointer(i, m_Elements[i].dataCount, m_Elements[i].dataType, m_Elements[i].normalized, m_Elements[i].bytesPerVertex, m_Elements[i].bytesPerItemStart); // offsetof(position, "my second item in Vertex class")
	//}

	// Create vertex buffer
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = (D3D11_USAGE)m_BufferUsage;
	vbd.CPUAccessFlags = 0;
	if ((D3D11_USAGE)m_BufferUsage == D3D11_USAGE_DYNAMIC)
		vbd.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.ByteWidth = m_Vertices.size() * sizeof(float);
	vbd.StructureByteStride = 3 * sizeof(float); // 3 for x, y, z
	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = m_Vertices.data();
	HRESULT hr;
	DXCallHR(Renderer::GetDevice()->CreateBuffer(&vbd, &vsd, &m_RendererPtr));
	
	// Bind vertex buffer to pipeline
	const UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, m_RendererPtr.GetAddressOf(), &m_Elements[0].bytesPerVertex, &offset);
	//class VertexBufferLayout
	//{
	//public:
	//	unsigned int itemID;			// 0 - position, 1 - texture coordinates
	//	unsigned int dataCount;			// 3 for position, 2 for texture coordinates
	//	unsigned int bytesPerVertex;	// 3+2=5*sizeof(float) = the amount of bytes of data for a single vertex
	//	void* bytesPerItemStart;		// starting position for the item id, 0 - 3 * sizeof(float), 1 - 2 * sizeof(float) or just use offsetof(firstItem, secondItem)
	//	unsigned char normalized;		// GL_FALSE
	//	int dataType;					// GL_FLOAT, GL_UNSIGNED_INT
	//};

	// input (vertex) layout
	WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;
	for (uint8_t i = 0; i < m_LayoutItemCount; i++)
	{
		// TODO: this is hardcoded af
		D3D11_INPUT_ELEMENT_DESC ied = {};
		ied.SemanticName = "Position";
		ied.SemanticIndex = 0;
		ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied.InputSlot = 0; // not important
		ied.AlignedByteOffset = 0;
		// Instancing stuff (not important)
		ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied.InstanceDataStepRate = 0;

		ieds.push_back(ied);
	}
	// NOTE: second issue, this function requires bytecode blobs from shaders
	// future workaround: idfk (maybe expose shader getters and setters and pass it into this func?)
	// current retarded acoustic workaround: move this into the main function
	//Renderer::GetDevice()->CreateInputLayout(ieds.data(), ieds.size(), );
}

void VertexBuffer::PushVertex(Vertex& vertex)
{
	m_Vertices.push_back(vertex.position.x);
	m_Vertices.push_back(vertex.position.y);
	m_Vertices.push_back(vertex.position.z);
}

void VertexBuffer::FinalizeVertices()
{
	//Bind();
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), m_Vertices.data(), m_BufferUsage);
}

void VertexBuffer::Bind() const
{
	//GLBindCache& cache = GLBindCache::GetInstance(BufferType::VertexBuffer);
	//
	////if (cache.BufferBound(m_RendererID))
	//	//return;
	//
	//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//cache.BufferCache(m_RendererID);

	// Bind vertex buffer to pipeline
	const UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, m_RendererPtr.GetAddressOf(), &m_Elements[0].bytesPerVertex, &offset);
}

void VertexBuffer::Unbind()
{
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
	//glDeleteBuffers(1, &m_RendererID);
	bufferInstanceCount--;
}
#endif