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

#include "Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/Texture.h"



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
		
		/*float positions[]{//for when projection matrix is -1 to 1
			-0.5f,-0.5f, 0.0f, 0.0f,
			 0.5f,-0.5f, 1.0f, 0.0f,
			 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f
		};*/
		float positions[]{//for when projection matrix is 0 to width/height
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,	 50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));// the array, how many vertices there are, how many values per vertex, size of 1 value in bytes
		VertexBufferLayout layout;
		layout.Push<float>(2); //how many dimensions the thing is
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

// 		int gcd = (width, height); //get the greatest common divisor of something, TLDR magic
// 		std::cout << ((float)width / (float)gcd) << " this and this " << ((float)height / (float)gcd) << std::endl;
// 		glm::mat4 proj = glm::ortho(-((float)width / (float)gcd), (float)width / (float)gcd, -((float)height / (float)gcd), (float)height / (float)gcd, -1.0f, 1.0f); //make orthographic projection matrix with aspect ratio of window, TLDR more magic oh and also some culling
		glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1000.0f, 1000.0f); 
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //camera

		Shader shader("res/shaders/Basic");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.3f, 0.0f, 0.8f, 1.0f);

		Texture texture("res/textures/prototype.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

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
		//std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION);
		//ImGui_ImplOpenGL3_Init((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		float r = 0.0f;
		ImGui::GetStyle().WindowRounding = 5.0f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			renderer.Clear();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //camera
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				//shader.SetUniform4f("u_Color", (sin(r) + 1) / 2, 0.3f, (sin(r + 1) + 1), 1.0f); //static color
				shader.Bind();
				shader.setUniformMat4f("u_MVP", mvp); //give shader the model view projection matrix
				renderer.Draw(va, ib, shader);
			}
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.setUniformMat4f("u_MVP", mvp); //give shader the model view projection matrix
				renderer.Draw(va, ib, shader);
			}

			r += 0.05f;

			ImGui::SliderFloat3("Translation of square 1", &translationA.x, 0.0f, 1000.0f);            // Edit translationA
			ImGui::SliderFloat3("Translation of square 2", &translationB.x, 0.0f, 1000.0f);            // Edit translationB
			if (ImGui::Button("Exit"))                            //exit button
				break;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImGui::Render();
			ImGui::EndFrame();
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