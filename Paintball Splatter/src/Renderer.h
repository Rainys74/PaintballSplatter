#pragma once

#if 0
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
#else
#include "DX11Includes.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Window.h"

std::string HResultToString(HRESULT hr);
void DXMessageBoxError(const std::string& errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function);

#define DXCallHR(x) hr = (x); if (FAILED(hr)) {std::string errorMsg = HResultToString(hr); DXMessageBoxError(errorMsg, hr, __FILE__, __LINE__, __func__);}

class Renderer
{
public:
	static void Initialize(Engine::Window* window);
	static void SetViewport(int width, int height, int xPos = 0, int yPos = 0);
	static void Clear();
	static void ClearColor(float r, float g, float b, float a);
	static void Render(VertexBuffer& vbo, IndexBuffer& ibo, Shader& shader, Math::Matrix4x4 model);
	static void RenderTestDX11(VertexBuffer& vbo, IndexBuffer& ibo, Math::Matrix4x4 model);

	static Math::Matrix4x4 GetViewMatrix() { return m_View; }
	static Math::Matrix4x4 GetProjectionMatrix() { return m_Projection; }

	static void SetViewMatrix(Math::Matrix4x4 mat4) { m_View = mat4; }
	static void SetProjectionMatrix(Math::Matrix4x4 mat4) { m_Projection = mat4; }

	static WRL::ComPtr<ID3D11Device> GetDevice();
	static WRL::ComPtr<IDXGISwapChain> GetSwapChain();
	static WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();

	static WRL::ComPtr<ID3D11RenderTargetView> GetRenderTarget();
private:
	static Math::Matrix4x4 m_View;
	static Math::Matrix4x4 m_Projection;

	static WRL::ComPtr<ID3D11Device> m_Device;
	static WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	static WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	static WRL::ComPtr<ID3D11RenderTargetView> m_RenderTarget;
};
#endif