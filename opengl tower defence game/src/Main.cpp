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

#include "Renderer.h"
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

	GLCall(GLenum err = glewInit())
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

		// generate indices
		/*unsigned int indices[] = {
			// Front face
			0, 1, 2,
			2, 3, 0,

			// Back face
			4, 5, 6,
			6, 7, 4,

			// Top face
			8, 9, 10,
			10, 11, 8,

			// Bottom face
			12, 13, 14,
			14, 15, 12,

			// Right face
			16, 17, 18,
			18, 19, 16,

			// Left face
			20, 21, 22,
			22, 23, 20
		};*/
		Mesh cube;
		cube.loadModel("res/models/teapot.obj");
		/*unsigned int indices[MaxIndexCount];
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
		}*/

		// transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// depth test
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		VertexArray va;
		VertexBuffer vb(nullptr, sizeof(Vertex) * MaxVertexCount);// the array/nullptr, max size of the buffer(size of a vertex, max amount of vertices(how much vram to allocate)), OPTIONAL: type(for example: GL_DYNAMIC_DRAW is default)
		VertexBufferLayout layout;
		layout.Push<float>(3); //how many dimensions the thing is
		layout.Push<float>(3); //normal
		layout.Push<float>(2); //texture coordinates
		//layout.Push<float>(1); //texture ID
		va.AddBuffer(vb, layout);

		IndexBuffer ib(cube.indices, MaxIndexCount);
// 		std::cout << ((float)width / (float)gcd) << " this and this " << ((float)height / (float)gcd) << std::endl;
// 		glm::mat4 proj = glm::ortho(-((float)width / (float)gcd), (float)width / (float)gcd, -((float)height / (float)gcd), (float)height / (float)gcd, -1.0f, 1.0f); //make orthographic projection matrix with aspect ratio of window, TLDR more magic oh and also some culling
		glm::mat4 proj = glm::perspective(fov, aspectRatio, near, far);
		//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 1000.0f);
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //camera
		Camera camera1;

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
		const ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::GetStyle().WindowRounding = 5.0f;

		// setup variables for gui
		glm::vec3 translationA(0, 0, 0);
		glm::vec3 rotationA(0, 0, 0);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			renderer.Clear();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//setup vertex buffer
			/*auto q0 = mesh::CreateQuad(translationA.x, translationA.y, sizeA, 0);
			auto q1 = mesh::CreateQuad(translationB.x, translationB.y, sizeB, 1);
			mesh::Vertex vertices[8];
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
			}*/
			// Define the vertices for a cube
			/*mesh::Vertex vertices[] = {
				// Front face
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0 },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0 },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0 },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0 },

				// Back face
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0 },
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0 },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0 },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0 },

				// Top face
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0 },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0 },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0 },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec2(0.0f, 1.0f), 0 },

				// Bottom face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0 },
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec2(1.0f, 0.0f), 0 },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0 },
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0 },

				// Right face
				{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0 },
				{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0 },
				{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec2(1.0f, 1.0f), 0 },
				{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0 },

				// Left face
				{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0 },
				{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0 },
				{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0 },
				{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0 }
			};*/
			vb.Bind();
			//std::cout << (sizeof(cube.vertices[0]) * cube.vertices.size()) << " is the size of vertices" << std::endl;
			glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(cube.vertices[0]) * cube.vertices.size()), &cube.vertices[0]);

			glm::mat4 model = glm::mat4(1);
			camera1.SetPosition(translationA);
			camera1.SetRotation(rotationA);
			shader.Bind();
			shader.setUniformMat4f("u_Model", model); //give shader the model matrix
			shader.setUniformMat4f("u_View", camera1.GetViewMatrix()); //give shader the view matrix
			shader.setUniformMat4f("u_Proj", proj); //give shader the projection matrix
			renderer.Draw(va, ib, shader);

			ImGui::ShowUserGuide();
			//ImGui::ShowAboutWindow();
			ImGui::DragFloat3("Camera Position", &translationA.x, 1.0f); // Camera Position
			ImGui::DragFloat3("Camera Rotation", &rotationA.x, 0.1f);	 // Camera Rotation
			if (ImGui::Button("Exit"))									 // Exit Button
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