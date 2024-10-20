#pragma once
#include "Shader.h"

class Uniform
{
public:
	enum class UniformType
	{
		Float, Vector3, Vector4, Matrix4x4
	};

private:
	UniformType m_UniformType;
};

class Material
{
public:
	Material(Shader shader) : m_Shader(shader) {}
		// : m_Shader(shader), m_FloatUniforms({0}), m_Vec3Uniforms({0}) {}

	template<typename T>
	void SetUniform(std::string& name, T value)
	{
		PS_ASSERT(false, "Uniform type is unrecognised.");
	}

	void PushUniforms();
private:
	Shader m_Shader;
	//std::unordered_map<std::string&, Uniform&> m_Uniforms;
	//std::unordered_map<std::string&, float> m_FloatUniforms;
	//std::unordered_map<std::string&, Math::Vector3&> m_Vec3Uniforms;
};