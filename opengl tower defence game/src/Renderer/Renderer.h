#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include <iostream>
#include <array>
#include "Shader.h"
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(); //old error handling
bool GLLogCall(const char* function, const char* file, int line); //old error handling

void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void WindowResizeCallback(GLFWwindow * window, int width, int height);

int windowWidth = 1080;
int windowHeight = 720;

class Renderer
{
public:
	void Clear() const;
	void Clear(float red, float green, float blue, float alpha) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(Mesh& mesh, Shader& shader, bool setModelMatrix = true, bool setTexture = true);
	void InitOpenGL();

private:

};
