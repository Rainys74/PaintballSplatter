#include "Material.h"
#include "EngineMath.h"

void Material::PushUniforms()
{
	/*for (const auto& pair : m_FloatUniforms)
	{
		//pair.first; pair.second;
	}
	for (const auto& pair : m_Vec3Uniforms)
	{
		//pair.first; pair.second;
	}*/
}

template<>
void Material::SetUniform(std::string& name, float value)
{
	//m_FloatUniforms[name] = value;
}

template<>
void Material::SetUniform(std::string& name, int value)
{
	
}

template<>
void Material::SetUniform(std::string& name, Math::Vector3 value)
{

}

template<>
void Material::SetUniform(std::string& name, Math::Vector4 value)
{

}

template<>
void Material::SetUniform(std::string& name, Math::Matrix4x4 value)
{

}