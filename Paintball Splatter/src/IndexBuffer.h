#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count); // may use shorts for smaller models
	~IndexBuffer();

	void Bind() const;
	static void Unbind();
	void Delete();

	inline unsigned int GetCount() const { return m_Count; }
};