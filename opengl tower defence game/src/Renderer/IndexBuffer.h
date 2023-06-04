#pragma once
#include <vector>

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const std::vector<unsigned int>& data, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Set(const std::vector<unsigned int>& data, unsigned int count);

	inline unsigned int GetCount() const { return m_Count; }
};
