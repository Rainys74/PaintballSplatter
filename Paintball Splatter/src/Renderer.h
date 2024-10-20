#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	static void Initialize();
	static void SetViewport(int width, int height, int xPos = 0, int yPos = 0);
	static void Clear();
	static void ClearColor(float r, float g, float b, float a);
	static void Render(VertexBuffer& vbo, IndexBuffer& ibo, Shader& shader, Math::Matrix4x4 model);

	static Math::Matrix4x4 GetViewMatrix() { return m_View; }
	static Math::Matrix4x4 GetProjectionMatrix() { return m_Projection; }

	static void SetViewMatrix(Math::Matrix4x4 mat4) { m_View = mat4; }
	static void SetProjectionMatrix(Math::Matrix4x4 mat4) { m_Projection = mat4; }
private:
	static Math::Matrix4x4 m_View;
	static Math::Matrix4x4 m_Projection;
};