#pragma once
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind();
	void unbind();

	unsigned int GetCount() const { return m_Count; }
};

