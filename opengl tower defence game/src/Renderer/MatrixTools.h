#pragma once
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace MatrixTools
{
	glm::mat4 GetProjectionMatrix(float width, float height, float fov, float near, float far) // fov is in degrees
	{
		return glm::perspective(glm::radians(fov), width / height, near, far);
	}
}