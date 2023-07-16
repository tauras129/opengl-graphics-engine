#pragma once
#include <GLM/glm.hpp>
#include <GLM/detail/type_quat.hpp>

class Camera
{
public:
	Camera(glm::vec3 translation = glm::vec3(1), glm::quat rotation = glm::quat(glm::vec3(0)), glm::vec3 scale = glm::vec3(1));
	~Camera();

	//glm::mat4 GetModelMatrix();
	glm::mat4 GetViewMatrix();
	//glm::mat4 GetProjectionMatrix();
	glm::vec3 GetPosition();
	glm::quat GetRotation();

	void GlobalMove(glm::vec3 translation);
	void LocalMove(glm::vec3 translation);
	void Rotate(glm::quat rotation);
	void Scale(glm::vec3 scale);

	void SetPosition(glm::vec3 translation);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	//void SetModelMatrix(glm::mat4 modelMatrix);
	//void SetProjectionMatrix(glm::mat4 projectionMatrix);
	void SetViewMatrix(glm::mat4 const &viewMatrix);
private:
	glm::mat4 view; // the view matrix of the camera
	glm::vec3 translation; // the translation of the camera
	glm::quat rotation; // the rotation of the camera
	glm::vec3 scale; // the scale of the camera
};
