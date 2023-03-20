#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <numeric>
#include <array>

#include "Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/Texture.h"

struct Vertex
{
	float Position[3];
	float Color[4];
	float TexCoords[2];
	float TexID;
};

static std::array<Vertex, 4> CreateQuad(float x, float y, float size, float textureID, const std::array<float, 4>& color = { 0.0f, 0.0f, 1.0f, 1.0f })
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

	GLCall(GLenum err = glewInit());
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
	{

// 		unsigned int indices[] = {
// 			0, 1, 2,
// 			2, 3, 0,
// 
// 			4, 5, 6,
// 			6, 7, 4
// 		};

		const int MaxQuadCount = 1000;
		const int MaxVertexCount = MaxQuadCount * 4;
		const int MaxIndexCount = MaxQuadCount * 6;

		// generate indices
		unsigned int indices[MaxIndexCount];
		unsigned int offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;
			offset += 4;
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		VertexArray va;
		VertexBuffer vb(nullptr, sizeof(Vertex) * MaxVertexCount);// the array/nullptr, max size of the buffer(size of a vertex, max amount of vertices(how much vram to allocate)), OPTIONAL: type(for example: GL_DYNAMIC_DRAW is default)
		VertexBufferLayout layout;
		layout.Push<float>(3); //how many dimensions the thing is
		layout.Push<float>(4); //color(rgba)
		layout.Push<float>(2); //texture coordinates
		layout.Push<float>(1); //texture ID
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, MaxIndexCount);

// 		int gcd = (width, height); //get the greatest common divisor of something, TLDR magic
// 		std::cout << ((float)width / (float)gcd) << " this and this " << ((float)height / (float)gcd) << std::endl;
// 		glm::mat4 proj = glm::ortho(-((float)width / (float)gcd), (float)width / (float)gcd, -((float)height / (float)gcd), (float)height / (float)gcd, -1.0f, 1.0f); //make orthographic projection matrix with aspect ratio of window, TLDR more magic oh and also some culling
		glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1000.0f, 1000.0f); 
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //camera

		Shader shader("res/shaders/Basic");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.3f, 0.0f, 0.8f, 1.0f);

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
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);

		// setup variables for gui
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);
		float sizeA = 50.0f;
		float sizeB = 50.0f;
		glm::vec3 translationC(0, 0, 0);

		ImGui::GetStyle().WindowRounding = 5.0f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			renderer.Clear();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//setup vertex buffer
			auto q0 = CreateQuad(translationA.x, translationA.y, sizeA, 0.0f);
			auto q1 = CreateQuad(translationB.x, translationB.y, sizeB, 1.0f);
			Vertex vertices[8];
			for (int i = 0; i < sizeof(vertices)/sizeof(vertices[0]); i++)
			{
				if (i < 4)
				{
					vertices[i] = q1[i];
				}
				else
				{
					vertices[i] = q0[i - 4];
				}
			}
			vb.Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			
			glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //camera
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationC);
 			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.setUniformMat4f("u_MVP", mvp); //give shader the model view projection matrix
			renderer.Draw(va, ib, shader);

			ImGui::SliderFloat3("Translation of square 1", &translationA.x, 0.0f, 1000.0f);     // Edit translationA
			ImGui::SliderFloat3("Translation of square 2", &translationB.x, 0.0f, 1000.0f);     // Edit translationB
			ImGui::SliderFloat("Size of square 1", &sizeA, 0.0f, 500.0f);						// Edit sizeA
			ImGui::SliderFloat("Size of square 2", &sizeB, 0.0f, 500.0f);						// Edit sizeB
			ImGui::DragFloat3("Translation of render 1", &translationC.x, 5.0f);				// Edit translationC
			if (ImGui::Button("Exit"))															//exit button
				break;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

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