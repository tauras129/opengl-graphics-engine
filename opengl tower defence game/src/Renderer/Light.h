#pragma once
#include "VertexBufferLayout.h"
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "VertexArray.h"
#include "IndexBuffer.h"

class Light
{
private:
	//unsigned int m_RendererID;

	VertexBufferLayout layout;


	glm::vec3 translation = glm::vec3(0);
	glm::quat rotation = glm::quat(glm::vec3(0));
	glm::vec3 scale = glm::vec3(1);

	glm::mat4 modelMatrix;

	void updateModelMatrix() {
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(0.2f), scale);
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

public:
	VertexArray va;
	VertexBuffer vb = VertexBuffer(nullptr, 0);
	IndexBuffer ib;

	glm::vec3 lightPosition = glm::vec3(0.0f);

//	Shader lightShader = Shader("res/shaders/Light");
	Shader lightCubeShader = Shader("res/shaders/Light.vert", "res/shaders/LightObject.frag");
	glm::mat4 GetModelMatrix() const { return modelMatrix; }

	Light();
	~Light();
};

