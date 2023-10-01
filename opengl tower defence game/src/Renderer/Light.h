#pragma once
#include "VertexBufferLayout.h"
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"

struct LightProperties {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

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

	glm::vec3 GetTranslation() const { return lightProperties.position; }
	glm::quat GetRotation() const { return rotation; }
	glm::vec3 GetScale() const { return scale; }

	void SetTranslation(glm::vec3 translation) { lightProperties.position = translation; this->translation = translation; updateModelMatrix(); }
	void SetRotation(glm::quat rotation) { this->rotation = rotation; updateModelMatrix(); }
	void SetScale(glm::vec3 scale) { this->scale = scale; updateModelMatrix(); }

	void SetAmbient(glm::vec3 ambient) { lightProperties.ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse) { lightProperties.diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular) { lightProperties.specular = specular; }

	void SetConstant(float constant) { lightProperties.constant = constant; } // DO NOT TOUCH THIS JUST KEEP IT AT 1.0f
	void SetLinear(float linear) { lightProperties.linear = linear; }
	void SetQuadratic(float quadratic) { lightProperties.quadratic = quadratic; }

	void SetValues(Shader& shader, const Camera& camera) const;
	void SetValues(Shader& shader, const glm::mat4& projectionMatrix) const;
	void SetValues(Shader& shader) const;
	void SetValues(Shader& shader, const Camera& camera, const glm::mat4& projectionMatrix) const;

	LightProperties GetProperties() const { return lightProperties; }

	LightProperties lightProperties;

//	Shader lightShader = Shader("res/shaders/Light");
	Shader lightCubeShader = Shader("res/shaders/Light.vert", "res/shaders/LightObject.frag");
	glm::mat4 GetModelMatrix() const { return modelMatrix; }
	VertexBufferLayout GetVertexBufferLayout() const { return layout; }

	Light();
	~Light();
};

