#include "Renderer.h"

#if 0
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
#else
#include "logger.h"
#include "Window.h"

#include <string>
#include <sstream>
#include <bitset>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Math::Matrix4x4 Renderer::m_View;
Math::Matrix4x4 Renderer::m_Projection;

WRL::ComPtr<ID3D11Device> Renderer::m_Device;
WRL::ComPtr<IDXGISwapChain> Renderer::m_SwapChain;
WRL::ComPtr<ID3D11DeviceContext> Renderer::m_DeviceContext;

WRL::ComPtr<ID3D11RenderTargetView> Renderer::m_RenderTarget;

/*static*/ std::string HResultToString(HRESULT hr)
{
	if (HRESULT_FACILITY(hr) == FACILITY_WIN32) 
	{
		// If the error is from Win32, we need to strip the facility portion
		hr = HRESULT_CODE(hr);
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr
	);

	std::string message;
	if (size) 
	{
		message = messageBuffer;
		LocalFree(messageBuffer); // Free the buffer allocated by FormatMessage
	}
	else 
	{
		std::stringstream ss;
		ss << "Unknown error code: 0x" << std::hex << hr;
		message = ss.str();
	}

	return message;
}
/*static*/ void DXMessageBoxError(const std::string& errorMsg, const HRESULT hr, const char* file, const unsigned int line, const char* function)
{
	std::stringstream ss;
	ss << "Error Code: 0x" << std::hex << hr << " = " << std::dec << hr << " = 0o" << std::oct << hr << " = 0b" << std::bitset<32>(hr);
	ss << '\n' << '\n';
	ss << "[Error Message]:\n";
	ss << errorMsg.c_str();
	ss << '\n';
	ss << "[In]:\n";
	ss << file << "(" << line << "): " << function << "();";

	MessageBoxA(nullptr, ss.str().c_str(), "DXCallHR Failed", MB_ICONERROR);
}

//#define DXCallHR(x) hr = (x); if (FAILED(hr)) {std::string errorMsg = HResultToString(hr); DXMessageBoxError(errorMsg, hr, __FILE__, __LINE__, __func__);}

void Renderer::Initialize(Engine::Window* window)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = (HWND)window->Win32TryGetHWND();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0x00000000;

	HRESULT hr;

	// create device and front/back buffers, and swap chain and rendering context
	DXCallHR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE /*D3D_DRIVER_TYPE_UNKNOWN*/, nullptr, D3D11_CREATE_DEVICE_DEBUG /*swapflagslvalue |= D3D11_CREATE_DEVICE_DEBUG*/, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext));

	// gain access to texture subresource in swap chain (back buffer)
	WRL::ComPtr<ID3D11Resource> backBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);

	m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTarget);
}
void Renderer::SetViewport(int width, int height, int xPos, int yPos)
{
	/*// Release the current render target view and depth stencil view (Unbinds RTV)
	m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	// Resize Swapchain
	m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0x00000000);

	// Recreate the render target view (RTV)
	// TODO: this is the same as in init so probably create a static function for this or smth
	WRL::ComPtr<ID3D11Resource> backBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);

	m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTarget);*/

	// By yours (ChatGPT) Truly.
	// In OpenGL, (xPos, yPos) refers to the lower-left corner.
	// In DirectX, (xPos, yPos) refers to the top-left corner, so we need to invert yPos.

	//int adjustedYPos = height - yPos - 1; // Invert y-coordinate
	// -------------------------

	// Configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = xPos;
	vp.TopLeftY = yPos;
	m_DeviceContext->RSSetViewports(1, &vp);
}
void Renderer::Clear()
{

}
void Renderer::ClearColor(float r, float g, float b, float a)
{
	const float colors[] = { r, g, b, a };
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget.Get(), colors);
}
#include "GL/glew.h"
void Renderer::Render(VertexBuffer& vbo, IndexBuffer& ibo, Shader& shader, Math::Matrix4x4 model)
{
	//vbo.Bind();
	//ibo.Bind();
	//shader.Bind();
	//vbo.FinalizeLayout();

	Math::Matrix4x4 mvp = GetProjectionMatrix() * GetViewMatrix() * model;
	m_DeviceContext->DrawIndexed(ibo.GetCount(), 0, 0);
}
void Renderer::RenderTestDX11(VertexBuffer& vbo, IndexBuffer& ibo, Math::Matrix4x4 model)
{
	vbo.Bind();
	ibo.Bind();
	//vbo.FinalizeLayout();

	Math::Matrix4x4 mvp = GetProjectionMatrix() * GetViewMatrix() * model;
	m_DeviceContext->DrawIndexed(ibo.GetCount(), 0, 0);
}
WRL::ComPtr<ID3D11Device> Renderer::GetDevice()
{
	return m_Device;
}
WRL::ComPtr<IDXGISwapChain> Renderer::GetSwapChain()
{
	return m_SwapChain;
}
WRL::ComPtr<ID3D11DeviceContext> Renderer::GetDeviceContext()
{
	return m_DeviceContext;
}
WRL::ComPtr<ID3D11RenderTargetView> Renderer::GetRenderTarget()
{
	return m_RenderTarget;
}
#endif