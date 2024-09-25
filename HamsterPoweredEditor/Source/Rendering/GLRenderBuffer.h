#pragma once
#include <cstdint>


class GLRenderBuffer
{
public:
        GLRenderBuffer(int width, int height, int samples, uint32_t internalFormat);
	~GLRenderBuffer();

	bool Create(int width, int height, int samples, uint32_t internalFormat);
	void Bind();
	void Unbind();

	uint32_t GetID() const { return m_renderBufferId; }

private:
	uint32_t m_renderBufferId;
	int m_width;
	int m_height;
	int m_samples;
	uint32_t m_internalFormat;
};

