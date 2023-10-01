#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include <iostream>
#include <array>
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "Light.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(); //old error handling
bool GLLogCall(const char* function, const char* file, int line); //old error handling

void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void WindowResizeCallback(GLFWwindow * window, int width, int height);

inline int windowWidth = 1080;
inline int windowHeight = 720;

class Renderer
{
public:
	void Clear() const;
	void Clear(float red, float green, float blue, float alpha) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(Mesh& mesh, Shader& shader, bool setModelMatrix = true, bool setTexture = true, bool setSpecularTexture = true, bool setShininess = true, const std::string& textureName = "u_Material.diffuse", const std::string& specularTextureName = "u_Material.specular", const std::string& shininessName = "u_Material.shininess");
	void Draw(Light& light, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
	void InitOpenGL();

private:

};
