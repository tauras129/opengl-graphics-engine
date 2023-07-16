#include "Texture.h"
#include "../Vendor/stb_image.h"
#include <iostream>
#include "Renderer.h"

std::unordered_map<std::string, std::pair<TextureInfo, unsigned int>>Texture::textureCache;

Texture::Texture()
{

}

void Texture::Set(const std::string& path, const GLint scaleType /* = GL_NEAREST */)
{
	m_FilePath = path;

	// Check if the texture is already in the cache
	if (!Texture::textureCache.empty())
	{
		// Texture is present in the cache
		auto iter = Texture::textureCache.find(path);
		if (iter != Texture::textureCache.end()) {
			m_textureInfo = iter->second.first;
			m_RendererID = iter->second.second;

			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleType);		// REQUIRED | how to downscale the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleType);		// REQUIRED | how to upscale the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture

			Unbind();
		}
	}
	else
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_textureInfo.Width, &m_textureInfo.Height, &m_textureInfo.BPP, 4);
		glGenTextures(1, &m_RendererID);

		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleType);		// REQUIRED | how to downscale the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleType);		// REQUIRED | how to upscale the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture

		if (m_LocalBuffer)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_textureInfo.Width, m_textureInfo.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
			Unbind();
			stbi_image_free(m_LocalBuffer);
			Texture::textureCache[path] = std::make_pair(m_textureInfo, m_RendererID);
		}
		else
		{
			std::cout << "\nError: Failed to load texture " << path << std::endl;
			std::cout << stbi_failure_reason() << std::endl;
			__debugbreak();
		}
	}
}

Texture::Texture(const std::string& path, const GLint scaleType /* = GL_NEAREST */)
	: m_FilePath(path)

{

	// Check if the texture is already in the cache
	if (!Texture::textureCache.empty())
	{
		// Texture is present in the cache
		auto iter = Texture::textureCache.find(path);
		if (iter != Texture::textureCache.end()) {
			m_textureInfo = iter->second.first;
			m_RendererID = iter->second.second;

			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleType);		// REQUIRED | how to downscale the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleType);		// REQUIRED | how to upscale the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture

			Unbind();
		}
	}
	else
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_textureInfo.Width, &m_textureInfo.Height, &m_textureInfo.BPP, 4);

		if (m_LocalBuffer)
		{
			glGenTextures(1, &m_RendererID);

			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleType);		// REQUIRED | how to downscale the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleType);		// REQUIRED | how to upscale the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// REQUIRED | how to wrap the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_textureInfo.Width, m_textureInfo.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
			Unbind();
			stbi_image_free(m_LocalBuffer);
			Texture::textureCache.emplace(std::make_pair(path, std::make_pair(m_textureInfo, m_RendererID)));
		}
		else
		{
			std::cout << "\nError: Failed to load texture " << path << std::endl;
			std::cout << stbi_failure_reason() << std::endl;
			__debugbreak();
		}
	}

}

Texture::~Texture()
{
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	//glBindTextureUnit(1, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
