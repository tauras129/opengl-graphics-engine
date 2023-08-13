#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	window = glfwCreateWindow(windowWidth, windowHeight, "opengl tower defense", nullptr, nullptr);
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

	glfwSetWindowSizeCallback(window, WindowResizeCallback);
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

		// transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// depth test
		glEnable(GL_DEPTH_TEST);

		constexpr float fov = 90.0f; // field of view in degrees
		float near = 0.1f; // near clipping plane
		float far = 100000.0f; // far clipping plane
		glm::mat4 proj = MatrixTools::GetProjectionMatrix(windowWidth, windowHeight, fov, near, far);

		Mesh object;
		object.loadModel("res/models/cube.obj"); // "res/models/bunny/bunny.obj"

		Camera camera1;

		Shader shader("res/shaders/Basic");
		Shader litObject("res/shaders/Light");
		Shader setColor("res/shaders/Color");

		std::vector<Texture> textures;
		textures.emplace_back(Texture("res/textures/prototype.png"));
		textures.emplace_back(Texture("res/textures/prototype1.png"));
		textures.emplace_back(Texture("res/models/bunny/texture_standard.jpg"));
		textures.emplace_back(Texture("res/models/bunny/texture_ceramic.jpg"));
		textures.emplace_back(Texture("res/models/dog/texture.png"));
		textures.emplace_back(Texture("res/models/spooky thing/texture.png"));
		textures.emplace_back(Texture("res/models/spooky thing/normal.png"));
		textures.emplace_back(Texture("res/models/tiger/texture.png"));
		textures.emplace_back(Texture("res/models/tiger/texture-white.png"));
		textures.emplace_back(Texture("res/models/texture coord test/texcoord test.png"));
		textures.emplace_back(Texture("res/textures/crateDiffuse.png"));
		textures.emplace_back(Texture("res/textures/crateSpecular.png"));

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

		glm::vec4 clearColor(0.2f, 0.3f, 0.8f, 1.0f);

		int texID = 0;
		int specularTexID = 0;
		float moveSpeed = 1.0f;
		float rotateSpeed = 0.1f;
		float scaleSpeed = 0.25f;

		bool pOpen = true;

		std::vector<Mesh> objects;
		std::vector<unsigned int> specularTextureIDS;

		double prevTime = 0.0;
		double crntTime = 0.0;
		double timeDiff;
		unsigned int counter = 0;

		double FPS = 0;
		double MS = 0;

		bool vSyncEnabled = true;
		bool wireframeEnabled = false;
		glm::vec4 wireframeColor(1.0f, 1.0f, 1.0f, 1.0f);
		glfwSwapInterval(1);

		Light light;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{

			// get the fps and put it in the window title
			crntTime = glfwGetTime();
			timeDiff = crntTime - prevTime;
			counter++;
			if (timeDiff >= 1.0 / 30.0) // run every third of a second
			{
				FPS = (1.0 / timeDiff) * counter; // get fps
				MS = (timeDiff / counter) * 1000; // get milliseconds between frames
				std::string newTitle = "opengl tower defense - FPS: " + std::to_string(FPS) + " MS: " + std::to_string(MS) + " -"; // make the string to use as a window title
				glfwSetWindowTitle(window, newTitle.c_str()); // set the window title
				prevTime = crntTime;
				counter = 0;

				// Recalculate the projection matrix TODO: make it not do this every 3 frames
				proj = MatrixTools::GetProjectionMatrix(windowWidth, windowHeight, fov, near, far);

				// std::cout << windowHeight << std::endl;
			}


			/* Render here */
			renderer.Clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Configuration", &pOpen);
			if (pOpen == false) break;

			camera1.SetPosition(translationA);
			camera1.SetRotation(rotationA);
			camera1.SetScale(scaleA);

			shader.Bind();
			shader.SetUniformMat4f("u_View", camera1.GetViewMatrix());  // give shader the view matrix
			shader.SetUniformMat4f("u_Proj", proj);						// give shader the projection matrix

			litObject.Bind();
			litObject.SetUniformMat4f("u_View", camera1.GetViewMatrix());	// give shader the view matrix
			litObject.SetUniformMat4f("u_Proj", proj);						// give shader the projection matrix
			litObject.SetUniform3f("u_ViewPos", camera1.GetPosition().x, camera1.GetPosition().y, camera1.GetPosition().z);
			//litObject.SetUniform3f("u_Material.specular", 0.633f, 0.727811f, 0.633f);
			litObject.SetUniform1f("u_Material.shininess", 76.8f); // the current material is emerald (http://devernay.free.fr/cours/opengl/materials.html)
			litObject.SetUniform3f("u_Light.position", light.lightPosition.x, light.lightPosition.y, light.lightPosition.z);
			litObject.SetUniform3f("u_Light.ambient", 0.2f, 0.2f, 0.2f);
			litObject.SetUniform3f("u_Light.diffuse", 0.5f, 0.5f, 0.5f);
			litObject.SetUniform3f("u_Light.specular", 1.0f, 1.0f, 1.0f);

			setColor.Bind();
			setColor.SetUniformMat4f("u_View", camera1.GetViewMatrix());// give shader the view matrix
			setColor.SetUniformMat4f("u_Proj", proj);					// give shader the projection matrix
			setColor.SetUniform4f("u_ColorIn", wireframeColor.r, wireframeColor.g, wireframeColor.b, wireframeColor.a); // give shader the color of the wireframe

			for (int i = 0; i < objects.size(); i++)
			{
				if (wireframeEnabled)
					renderer.Draw(objects[i], setColor);

				else renderer.Draw(objects[i], litObject);
			}

			renderer.Draw(light, proj, camera1.GetViewMatrix());

			if (ImGui::CollapsingHeader("Display"))
			{
				if (ImGui::Checkbox("Enable VSync", &vSyncEnabled))
				{
					// anything here is executed when the checkbox is clicked
					glfwSwapInterval(vSyncEnabled);
				}

				if (ImGui::Checkbox("Wireframe", &wireframeEnabled))
				{
					// anything here is executed when the checkbox is clicked
					if (wireframeEnabled)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}
					else
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
				}

				if (wireframeEnabled)
				{
					ImGui::ColorEdit4("Wireframe Color", &wireframeColor.r, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar);
				}
			}

			if (ImGui::CollapsingHeader("Environment"))
			{
				ImGui::ColorEdit4("Background Color", &clearColor.r, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar);
			}

			if (ImGui::CollapsingHeader("Sliders"))
			{
				ImGui::DragFloat("Movement Speed", &moveSpeed, 0.01f, 0.01f, 100.0f);	// Movement Speed
				ImGui::DragFloat("Rotation Speed", &rotateSpeed, 0.01f, 0.01f, 10.0f);  // Rotation Speed
				ImGui::DragFloat("Scale Speed", &scaleSpeed, 0.01f, 0.01f, 10.0f);		// Scale Speed
			}

			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::DragFloat3("Position", &translationA.x, moveSpeed);	// Camera Position
				ImGui::DragFloat3("Rotation", &rotationA.x, rotateSpeed);	// Camera Rotation
				ImGui::DragFloat3("Scale(broken)", &scaleA.x, scaleSpeed);	// Camera Scale
			}

			if (ImGui::Button("New Object"))
			{
				objects.push_back(object);  // New Object Button
				specularTextureIDS.push_back(11);
			}
			if (ImGui::Button("100 New Objects")) for (int i = 0; i < 100; i++)
			{
				objects.push_back(object);  // 100 New Objects Button
				specularTextureIDS.push_back(11);
			}
			if (ImGui::CollapsingHeader("Objects"))
			{
				for (int i = 0; i < objects.size(); i++)
				{
					std::string name = "Object " + std::to_string(i);
					if (ImGui::CollapsingHeader(name.c_str()))
					{
						std::string positionName = "Position##Position" + std::to_string(i);
						std::string rotationName = "Rotation##Rotation" + std::to_string(i);
						std::string scaleName = "Scale##Scale" + std::to_string(i);
						std::string texIDName = "Texture##Texture" + std::to_string(i);
						std::string specularTexIDName = "Specular Texture##TextureSpecular" + std::to_string(i);
						std::string deleteName = "Delete object##Del" + std::to_string(i);

						translationB = objects[i].GetTranslation();
						rotationB = glm::eulerAngles(objects[i].GetRotation());
						scaleB = objects[i].GetScale();
						texID = objects[i].GetTextureID();
						specularTexID = specularTextureIDS[i];

						ImGui::DragFloat3(positionName.c_str(), &translationB.x, moveSpeed);			 // Object Position
						ImGui::DragFloat3(rotationName.c_str(), &rotationB.x, rotateSpeed);				 // Object Rotation
						ImGui::DragFloat3(scaleName.c_str(), &scaleB.x, scaleSpeed);					 // Object Scale
						ImGui::SliderInt(texIDName.c_str(), &texID, 1, textures.size());				 // Object Texture ID
						ImGui::SliderInt(specularTexIDName.c_str(), &specularTexID, 1, textures.size()); // Object Texture ID

						objects[i].SetPosition(translationB);
						objects[i].SetRotation(rotationB);
						objects[i].SetScale(scaleB);
						objects[i].SetTextureID(texID);
						objects[i].SetTexture(textures[texID - 1]);
						specularTextureIDS[i] = specularTexID;
						objects[i].SetSpecularTexture(textures[specularTexID - 1]);
						if (ImGui::Button(deleteName.c_str())) objects.erase(objects.begin() + i);  // Delete Object Button
					}
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