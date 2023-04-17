#pragma once

#include <GL/glew.h>
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include <iostream>
#include <array>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(); //old error handling
bool GLLogCall(const char* function, const char* file, int line); //old error handling

void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

/*namespace mesh
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		int TexID;
	};
	static std::array<Vertex, 4> CreateQuad(float x, float y, float size, int textureID, const std::array<float, 4>&color = { 0.0f, 0.0f, 1.0f, 1.0f })
	{

		Vertex v0 = {// first vertex
			{x, y, 0.0f},							 // Position
			{color[0], color[1], color[2], color[3]},// Color
			{0.0f, 0.0f},							 // TexCoords
			textureID								 // TexID
		};

		Vertex v1 = {// second vertex
			{ x + size, y, 0.0f },					 // Position
			{color[0], color[1], color[2], color[3]},// Color
			{ 1.0f, 0.0f },							 // TexCoords
			textureID								 // TexID
		};

		Vertex v2 = {// third vertex
			{ x + size, y + size, 0.0f },			 // Position
			{color[0], color[1], color[2], color[3]},// Color
			{ 1.0f, 1.0f },							 // TexCoords
			textureID								 // TexID
		};

		Vertex v3 = {// fourth vertex
			{ x, y + size, 0.0f },					 // Position
			{color[0], color[1], color[2], color[3]},// Color
			{ 0.0f, 1.0f },							 // TexCoords
			textureID								 // TexID
		};

		return { v0, v1, v2, v3 };
	}
}*/

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

private:

};
