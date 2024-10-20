#include "Renderer.h"
#include "logger.h"
#include "OpenGLDebugHandler.h"

Math::Matrix4x4 Renderer::m_View;
Math::Matrix4x4 Renderer::m_Projection;

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		//PS_LOGGER_ERROR("[OpenGL Error] " << "(" << id << "): " << message);
		GLError::PushError(source, type, id, severity, length, message, userParam, nullptr, -1);
		GLError::HandleErrors();
	}
	if (type == GL_DEBUG_TYPE_OTHER && severity == GL_DEBUG_SEVERITY_MEDIUM)
	{
		PS_LOGGER_WARN("[OpenGL Warning]: " << message);
	}
	if (type == GL_DEBUG_TYPE_OTHER && severity == GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		//PS_LOGGER_INFO("[OpenGL Info]: " << message);
	}
}

void Renderer::Initialize()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(DebugCallback, nullptr);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	//unsigned int m_VertexArray;
	//glGenVertexArrays(1, &m_VertexArray);
	//glBindVertexArray(m_VertexArray);
}
void Renderer::SetViewport(int width, int height, int xPos, int yPos)
{
	glViewport(xPos, yPos, width, height);
}
void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}
void Renderer::Render(VertexBuffer& vbo, IndexBuffer& ibo, Shader& shader, Math::Matrix4x4 model)
{
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	vbo.Bind();
	ibo.Bind();
	shader.Bind();
	vbo.FinalizeLayout();

	Math::Matrix4x4 mvp = GetProjectionMatrix() * GetViewMatrix() * model;
	GLDebug(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}