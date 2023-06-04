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
#include "Renderer/MatrixTools.h"

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

		constexpr float fov = 90.0f; // field of view in degrees
		float near = 0.1f; // near clipping plane
		float far = 100000.0f; // far clipping plane
		glm::mat4 proj = MatrixTools::GetProjectionMatrix(width, height, fov, near, far);

		Mesh object;
		object.loadModel("res/models/bunny/bunny.obj"); // "res/models/bunny/bunny.obj"

		// transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// depth test
		glEnable(GL_DEPTH_TEST);

		Camera camera1;

		Shader shader("res/shaders/Basic");
		shader.Bind();

		Texture texture1("res/textures/prototype.png");
		Texture texture2("res/textures/prototype1.png");
		Texture texture3("res/models/bunny/texture_standard.jpg");
		Texture texture4("res/models/bunny/texture_ceramic.jpg");
		Texture texture5("res/models/dog/texture.png");
		Texture texture6("res/models/spooky thing/texture.png");
		Texture texture7("res/models/spooky thing/normal.png");
		Texture texture8("res/models/tiger/texture.png");
		Texture texture9("res/models/tiger/texture-white.png");
		texture1.Bind(0);
		texture2.Bind(1);
		texture3.Bind(2);
		texture4.Bind(3);
		texture5.Bind(4);
		texture6.Bind(5);
		texture7.Bind(6);
		texture8.Bind(7);
		texture9.Bind(8);
		int textures[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
		shader.SetUniform1iv("u_Textures", textures, 9);

		shader.Unbind();

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
		glm::vec3 scaleA(1, 1, 1);

		glm::vec3 translationB(0, 0, 0);
		glm::vec3 rotationB(0, 0, 0);
		glm::vec3 scaleB(1, 1, 1);
		int texID = 0;
		float moveSpeed = 1.0f;
		float rotateSpeed = 0.1f;
		float scaleSpeed = 0.25f;

		bool pOpen = true;

		std::vector<Mesh> objects;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			renderer.Clear(0.2f, 0.3f, 0.8f, 1.0f);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Configuration", &pOpen);
			if (pOpen == false) break;

			camera1.SetPosition(translationA);
			camera1.SetRotation(rotationA);
			camera1.SetScale(scaleA);
			shader.Bind();
			shader.setUniformMat4f("u_View", camera1.GetViewMatrix());  // give shader the view matrix
			shader.setUniformMat4f("u_Proj", proj);						// give shader the projection matrix

			for (int i = 0; i < objects.size(); i++)
				renderer.Draw(objects[i], shader);


			ImGui::Text("Sliders");
			ImGui::DragFloat("Movement Speed", &moveSpeed, 0.01f, 0.01f, 100.0f);	// Movement Speed
			ImGui::DragFloat("Rotation Speed", &rotateSpeed, 0.01f, 0.01f, 10.0f);  // Rotation Speed
			ImGui::DragFloat("Scale Speed", &scaleSpeed, 0.01f, 0.01f, 10.0f);		// Scale Speed

			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &translationA.x, moveSpeed);	// Camera Position
			ImGui::DragFloat3("Rotation", &rotationA.x, rotateSpeed);	// Camera Rotation
			ImGui::DragFloat3("Scale(broken)", &scaleA.x, scaleSpeed);	// Camera Scale

			if (ImGui::Button("New Object")) objects.push_back(object);  // Delete Object Button
			for (int i = 0; i < objects.size(); i++)
			{
				std::string name = "Object " + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					std::string positionName = "Position##Position" + std::to_string(i);
					std::string rotationName = "Rotation##Rotation" + std::to_string(i);
					std::string scaleName    = "Scale##Scale"       + std::to_string(i);
					std::string texIDName    = "Texture##Texture"   + std::to_string(i);
					std::string deleteName   = "Delete object##Del" + std::to_string(i);

					translationB = objects[i].translation;
					rotationB = glm::eulerAngles(objects[i].rotation);
					scaleB = objects[i].scale;
					texID = objects[i].texID;

					ImGui::DragFloat3(positionName.c_str(), &translationB.x, moveSpeed);		// Object Position
					ImGui::DragFloat3(rotationName.c_str(), &rotationB.x, rotateSpeed);			// Object Rotation
					ImGui::DragFloat3(scaleName.c_str(), &scaleB.x, scaleSpeed);				// Object Scale
					ImGui::SliderInt(texIDName.c_str(), &texID, 0, 8);							// Object Texture ID

					objects[i].SetPosition(translationB);
					objects[i].SetRotation(rotationB);
					objects[i].SetScale(scaleB);
					objects[i].setTextureID(texID);
					if (ImGui::Button(deleteName.c_str())) objects.erase(objects.begin() + i);  // Delete Object Button
				}
			}

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