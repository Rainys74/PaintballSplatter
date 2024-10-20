#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

namespace Math
{
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Matrix4x4 = glm::mat4;

	template<typename T>
	Matrix4x4 Ortho(T left, T right, T bottom, T top, T zNear, T zFar) { return glm::ortho(left, right, bottom, top, zNear, zFar); }
}