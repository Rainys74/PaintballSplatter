#include "OpenGLDebugHandler.h"
#include "logger.h"

std::unordered_map<unsigned int, GLErrorData> GLError::m_Errors;

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR); //!glGetError()
}
bool GLLogCall(const char* function, const char* file, int line)
{
	while (unsigned int error = glGetError())
	{
		GLError::PushError(0, 0, error, 0, 0, nullptr, nullptr, file, line);
		GLError::HandleErrors();
		return false;
	}
	return true;
}

#define IS_MODERN_ERROR(x) x.file == nullptr && x.line == -1
void GLError::PushError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam, const char* file, int line)
{
	GLErrorData errorData;
	errorData.source = source;
	errorData.type = type;
	errorData.id = id;
	errorData.severity = severity;
	errorData.length = length;
	errorData.message = message;
	errorData.userParam = userParam;
	errorData.file = file;
	errorData.line = line;

	if (IS_MODERN_ERROR(errorData))
	{
		auto it = m_Errors.find(id);
		if (it == m_Errors.end())
		{
			m_Errors[id] = errorData;
		}
		else
		{
			m_Errors[id].source = errorData.source;
			m_Errors[id].type = errorData.type;
			m_Errors[id].severity = errorData.severity;
			m_Errors[id].length = errorData.length;
			m_Errors[id].message = errorData.message;
			m_Errors[id].userParam = errorData.userParam;
		}
	}
	else
	{
		auto it = m_Errors.find(id);
		if (it == m_Errors.end())
		{
			m_Errors[id] = errorData;
		}
		else
		{
			m_Errors[id].file = errorData.file;
			m_Errors[id].line = errorData.line;
		}
	}
}

void GLError::HandleErrors()
{
	for (const auto& error : m_Errors)
	{
		GLErrorData errorData = error.second;
		if (IS_MODERN_ERROR(errorData))
		{
			if (errorData.type == GL_DEBUG_TYPE_ERROR)
			{
				//PS_LOGGER_ERROR("[OpenGL Error] " << "(" << errorData.id << "): " << errorData.message);
				PS_LOGGER_ERROR("[OpenGL Error " << errorData.id << "] " << errorData.message);
			}
			if (errorData.type == GL_DEBUG_TYPE_OTHER && errorData.severity == GL_DEBUG_SEVERITY_MEDIUM)
			{
				PS_LOGGER_WARN("[OpenGL Warning]: " << errorData.message);
			}
			if (errorData.type == GL_DEBUG_TYPE_OTHER && errorData.severity == GL_DEBUG_SEVERITY_NOTIFICATION)
			{
				//PS_LOGGER_INFO("[OpenGL Info]: " << message);
			}
		}
		else
		{
			if (errorData.type == GL_DEBUG_TYPE_ERROR)
			{
				PS_LOGGER_ERROR("[OpenGL Error " << errorData.id << "] " << errorData.file << "(" << errorData.line << "): " << errorData.message);
			}
			if (errorData.type == GL_DEBUG_TYPE_OTHER && errorData.severity == GL_DEBUG_SEVERITY_MEDIUM)
			{
				PS_LOGGER_WARN("[OpenGL Warning]: " << errorData.message);
			}
			if (errorData.type == GL_DEBUG_TYPE_OTHER && errorData.severity == GL_DEBUG_SEVERITY_NOTIFICATION)
			{
				//PS_LOGGER_INFO("[OpenGL Info]: " << message);
			}
			//PS_LOGGER_ERROR("[OpenGL Error]: " << errorData.id << " On: " << errorData.file << "(" << errorData.line << ")");
		}
	}
	//m_Errors.clear();
}