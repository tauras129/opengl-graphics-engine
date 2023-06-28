#pragma once
#include <string>

#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601

class Texture
{
private:
	unsigned int m_RendererID = 0;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer = nullptr;
	int m_Width = 0;
	int m_Height = 0;
	int m_BPP = 0; // bytes per pixel
public:
	explicit Texture(const std::string& path, const int scaleType = GL_NEAREST);
	// path is the path to the texture in any of these formats: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC. The path to the texture is relative to where you run the program
	// or (untested) you can do something like "C:/users/user/images/something.png" but keep in mind that I do not know if this will work as I have not tried this.
	//
	// scaleType is the way to scale up/down the texture
	// use GL_NEAREST(the default) to scale a texture and make it stay the same resolution for example for 16x16 pixel art
	// you can also use GL_LINEAR to hide the pixels by blurring it.
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};