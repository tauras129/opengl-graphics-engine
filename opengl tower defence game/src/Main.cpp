#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <ASSIMP/Importer.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <numeric>
#include <array>

#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	int width  = 1080;
	int height = 720;
	window = glfwCreateWindow(width, height, "opengl tower defense", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << " and OpenGL " << glGetString(GL_VERSION) << " and GLFW " << glfwGetVersionString() << std::endl;
	int textureUnits = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
	std::cout << "Status: Max texture units: " << textureUnits << std::endl;
	{

		const int MaxQuadCount = 1;
		const int MaxVertexCount = MaxQuadCount * 2976;
		const int MaxIndexCount = MaxQuadCount * 6 * 6000;
		const float aspectRatio = (float)width / (float)height; // replace with your window's aspect ratio
		constexpr float fov = glm::radians(90.0f); // field of view in radians
		float near = 0.1f; // near clipping plane
		float far = 1000.0f; // far clipping plane

		Mesh cube;
		cube.loadModel("res/models/teapot.obj");

		// transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// depth test
		glEnable(GL_DEPTH_TEST);

		VertexArray va;
		VertexBuffer vb(nullptr, sizeof(Vertex) * MaxVertexCount);// the array/nullptr, max size of the buffer(size of a vertex, max amount of vertices(how much vram to allocate)), OPTIONAL: type(for example: GL_DYNAMIC_DRAW is default)
		VertexBufferLayout layout;
		layout.Push<float>(3); //how many dimensions the thing is
		layout.Push<float>(3); //normal
		layout.Push<float>(2); //texture coordinates
		layout.Push<unsigned int>(1); //texture ID
		va.AddBuffer(vb, layout);

		IndexBuffer ib(cube.indices, MaxIndexCount);
		glm::mat4 proj = glm::perspective(fov, aspectRatio, near, far);
		Camera camera1;

		Shader shader("res/shaders/Basic");
		shader.Bind();

		Texture texture1("res/textures/prototype.png");
		Texture texture2("res/textures/prototype1.png");
		texture1.Bind(0);
		texture2.Bind(1);
		int textures[2] = { 0, 1 };
		shader.SetUniform1iv("u_Textures", textures, 2);

		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		const ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::GetStyle().WindowRounding = 5.0f;

		// setup variables for gui
		glm::vec3 translationA(0, 0, 0);
		glm::vec3 rotationA(0, 0, 0);
		int texID = 0;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			renderer.Clear(0.2f, 0.3f, 0.8f, 1.0f);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Configuration", (bool*)true);

			cube.setTextureID(texID);
			vb.Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(cube.vertices[0]) * cube.vertices.size()), cube.vertices.data());

			glm::mat4 model = glm::mat4(1);
			camera1.SetPosition(translationA);
			camera1.SetRotation(rotationA);
			shader.Bind();
			shader.setUniformMat4f("u_Model", model);					// give shader the model matrix
			shader.setUniformMat4f("u_View", camera1.GetViewMatrix());  // give shader the view matrix
			shader.setUniformMat4f("u_Proj", proj);						// give shader the projection matrix
			renderer.Draw(va, ib, shader);

			ImGui::DragFloat3("Camera Position", &translationA.x, 1.0f);// Camera Position
			ImGui::DragFloat3("Camera Rotation", &rotationA.x, 0.1f);	// Camera Rotation
			ImGui::SliderInt("Object Texture", &texID, 0, 1);			// Object Texture ID
			if (ImGui::Button("Exit"))									// Exit Button
				break;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImGui::End();
			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}