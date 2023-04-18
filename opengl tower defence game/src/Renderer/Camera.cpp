#include "Camera.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/vector_angle.hpp>
#include <GLM/gtc/matrix_inverse.hpp>


Camera::Camera(glm::vec3 translation /*= glm::vec3(0)*/, glm::quat rotation /*= glm::quat()*/, glm::vec3 scale /*= glm::vec3(1)*/)
	: translation(translation), rotation(rotation), scale(scale)
{

// 	// yaw
// 	rotation = glm::normalize(glm::angleAxis(yaw, glm::vec3{ 0.f, -1.f, 0.f }) * rotation);
// 	// pitch
// 	rotation = glm::normalize(rotation * glm::angleAxis(glm::pitch, glm::vec3{ -1.f, 0.f, 0.f }));
// 	// r
// 	rotation = glm::normalize(rotation * glm::angleAxis(glm::roll, glm::vec3{ -1.f, 0.f, 0.f }));

	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
	view = glm::affineInverse(t * r * s);
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
	return view;
}
glm::vec3 Camera::GetPosition()
{
	return translation;
}
glm::quat Camera::GetRotation()
{
	return rotation;
}

void Camera::GlobalMove(glm::vec3 translation)
{
	this->translation += translation;
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.f), this->translation);
	glm::mat4 s = glm::scale(glm::mat4(1.f), scale);
	view = glm::affineInverse(t * r * s);
}
void Camera::LocalMove(glm::vec3 translation)
{
	this->translation += rotation * translation;
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.f), this->translation);
	glm::mat4 s = glm::scale(glm::mat4(1.f), scale);
	view = glm::affineInverse(t * r * s);
}

void Camera::Rotate(glm::quat rotation)
{

	this->rotation *= rotation;
	glm::mat4 r = glm::mat4_cast(this->rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.f), translation);
	glm::mat4 s = glm::scale(glm::mat4(1.f), scale);
	view = glm::affineInverse(t * r * s);

}

void Camera::SetPosition(glm::vec3 translation)
{
	this->translation = translation;
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.f), translation);
	glm::mat4 s = glm::scale(glm::mat4(1.f), scale);
	view = glm::affineInverse(t * r * s);


}

void Camera::SetRotation(glm::quat rotation)
{

	this->rotation = rotation;
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.f), translation);
	glm::mat4 s = glm::scale(glm::mat4(1.f), scale);
	view = glm::affineInverse(t * r * s);


}

void Camera::SetViewMatrix(glm::mat4 const &viewMatrix)
{
	view = viewMatrix;
}
