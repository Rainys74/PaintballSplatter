#pragma once

enum class BufferType
{
	VertexBuffer,
	IndexBuffer,
	Shader,
	VertexArray
};

class GLBindCache
{
public:
	GLBindCache()
		: m_BoundBuffer(0) {}

	static GLBindCache& GetInstance(BufferType objectType)
	{
		static GLBindCache instance;
		return instance;
	}

	bool BufferBound(unsigned int bufferID) const
	{
		if (bufferID == 0)
			return false;

		if (bufferID != m_BoundBuffer)
			return false;
		else
			return true;
	}
	void BufferCache(unsigned int bufferID)
	{
		m_BoundBuffer = bufferID;
	}

private:
	unsigned int m_BoundBuffer = 0; // std::unordered_map<GLenum, GLuint> bindings;
};