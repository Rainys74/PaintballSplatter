#pragma once
#if 0
#include <string>
#include <unordered_map>

struct ShaderProgram
{
	std::string& vertexShader, fragmentShader;
};

class Shader
{
public:
	Shader(std::string& shaderPath);
	Shader(std::string& vertexShader, std::string& fragmentShader);
	~Shader();

	int GetUniformLocation(const std::string& name);

	void Bind() const;
	static void Unbind();
	void Delete();

	//static std::string& GetVertexShader(std::string& shaderPath);
	//static std::string& GetFragmentShader(std::string& shaderPath);
	static ShaderProgram GetParsedShader(std::string& shaderPath);
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache; // if const this should be a mutable unordered_map
};
#else
#include "DX11Includes.h"

#include <string>
#include <unordered_map>

struct ShaderProgram
{
	std::string& vertexShader, fragmentShader;
};

class Shader
{
public:
	Shader(std::string& shaderPath);
	Shader(std::string& vertexShader, std::string& fragmentShader);
	~Shader();

	int GetUniformLocation(const std::string& name);

	void Bind() const;
	static void Unbind();
	void Delete();

	//static std::string& GetVertexShader(std::string& shaderPath);
	//static std::string& GetFragmentShader(std::string& shaderPath);
	static ShaderProgram GetParsedShader(std::string& shaderPath);
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache; // if const this should be a mutable unordered_map
};
#endif