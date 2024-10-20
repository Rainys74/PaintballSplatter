#pragma once

#include <unordered_map>

#include <GL/glew.h>

#ifdef PS_DEBUG
#define GLDebug(x) GLClearError(); x; GLLogCall(#x, __FILE__, __LINE__)
#else
#define GLDebug(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

struct GLErrorData
{
	GLenum source;
	GLenum type;
	GLuint id;
	GLenum severity;
	GLsizei length;
	const GLchar* message;
	const void* userParam;

	const char* file;
	int line;
};
class GLError
{
public:
	static void PushError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam, const char* file, int line);
	static void HandleErrors();
private:
	static std::unordered_map<unsigned int, GLErrorData> m_Errors;
};
