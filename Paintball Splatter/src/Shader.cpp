#include "Shader.h"
#include "logger.h"

#include <GL/glew.h>

#include <sstream>
#include <fstream>

#include "Buffer.h"

Shader::Shader(std::string& shaderPath)
	: m_RendererID(0)
{
	const char* vertexSource = "";
	const char* fragmentSource = "";

	m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::Shader(std::string& vertexShader, std::string& fragmentShader)
	: m_RendererID(0)
{
	const char* vertexSource = vertexShader.c_str();
	const char* fragmentSource = fragmentShader.c_str();

	m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader()
{
	Delete();
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		PS_LOGGER_WARN("Shader uniform (" << name << ") doesn't exist or is unused!");

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::Bind() const
{
	// TODO: make a bind caching system
	GLBindCache& cache = GLBindCache::GetInstance(BufferType::Shader);

	if (cache.BufferBound(m_RendererID))
		return;

	glUseProgram(m_RendererID);
	cache.BufferCache(m_RendererID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::Delete()
{
	glDeleteProgram(m_RendererID);
}

ShaderProgram Shader::GetParsedShader(std::string& shaderPath)
{
	std::ifstream stream(shaderPath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("@vertex") != std::string::npos)
		{
			// set mode to vertex
			type = ShaderType::VERTEX;
		}
		else if (line.find("@fragment") != std::string::npos)
		{
			// set mode to fragment
			type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return ShaderProgram{ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); //&source[0];
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// check for errors
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) // or !result
	{
		// query the error message length
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// construct stack allocated char array
		//char* message = (char*)malloc(length * sizeof(char));
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);
		// print error								if type == vertex, type vertex else type fragment
		PS_LOGGER_ERROR("Failed To Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader!");
		PS_LOGGER_ERROR(message);

		// delete shader since compilation failed
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attaching shaders
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// delete intermediate shaders after linking
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}