#include "Renderer.h"

#include <iostream>

void GLClearError() // old error handling
{
	while (glGetError() != GL_NO_ERROR) std::cout << glGetError() << std::endl;
}

bool GLLogCall(const char* function, const char* file, int line) // old error handling
{
	bool errorFound = false;
	while (GLenum error = glGetError())
	{
		// print out error
		std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ": " << line << std::endl;
		errorFound = true;
	}
	if (errorFound) return false;
	return true;
}

void Renderer::Clear() const
{
	// clears the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Clear(float red, float green, float blue, float alpha) const
{
	// clears the window with a color

	// check if the color values are lower that 0 or higher than 1, if they are warn the user
	if (red > 1) std::cout << "--WARNING-- red value " << red << " for renderer clear is higher that 1. are you using 0-255 instead of 0-1? If you are then divide it by 255, Good luck!" << std::endl;
	if (red < 0) std::cout << "--WARNING-- red value " << red << " for renderer clear is lower that 0. Good luck fixing this issue!" << std::endl;
	if (green > 1) std::cout << "--WARNING-- green value " << green << " for renderer clear is higher that 1. are you using 0-255 instead of 0-1? If you are then divide it by 255, Good luck!" << std::endl;
	if (green < 0) std::cout << "--WARNING-- green value " << green << " for renderer clear is lower that 0. Good luck fixing this issue!" << std::endl;
	if (blue > 1) std::cout << "--WARNING-- blue value " << blue << " for renderer clear is higher that 1. are you using 0-255 instead of 0-1? If you are then divide it by 255, Good luck!" << std::endl;
	if (blue < 0) std::cout << "--WARNING-- blue value " << blue << " for renderer clear is lower that 0. Good luck fixing this issue!" << std::endl;
	if (alpha > 1) std::cout << "--WARNING-- alpha value " << alpha << " for renderer clear is higher that 1. are you using 0-255 instead of 0-1? If you are then divide it by 255, Good luck!" << std::endl;
	if (alpha < 0) std::cout << "--WARNING-- alpha value " << alpha << " for renderer clear is lower that 0. Good luck fixing this issue!" << std::endl;
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

	// unbind stuff OPTIONAL, if performance is bad remove this
	shader.Unbind();
	va.Unbind();
	ib.Unbind();
}

void Renderer::Draw(Mesh& mesh, Shader& shader, bool setModelMatrix /*= true*/, bool setTexture /*= true*/)
// You do not need to set the shaders model matrix or texture, this function sets that(unless you change the parameter also setting the texture binds texture slot 0 to the meshes texture).
// You do still need to set the view and projection matrices.
{

	shader.Bind();
	mesh.va.Bind();
	mesh.ib.Bind();
	mesh.GetTexture().Bind();

	if(setModelMatrix) shader.SetUniformMat4f("u_Model", mesh.GetModelMatrix()); // give shader the model matrix
	if (setTexture) shader.SetUniform1i("u_Texture", 0); // give shader the models texture
	glDrawElements(GL_TRIANGLES, mesh.ib.GetCount(), GL_UNSIGNED_INT, nullptr);

}

void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "--OPENGL ERROR--" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
	__debugbreak();
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	// Update the viewport with the new window size
	glViewport(0, 0, width, height);

	windowWidth = width;
	windowHeight = height;
}
