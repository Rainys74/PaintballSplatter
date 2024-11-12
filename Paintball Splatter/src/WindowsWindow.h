#pragma once
#include "Window.h"

#include <Windows.h>

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow();
		virtual ~WindowsWindow() {}

		void Create(int width, int height, const std::string& title) override;
		void CreateOpenGLContext() override;
		void CreateOpenGLExtensions(OpenGLAttribs* glAttribs) override;
		int WindowShouldClose() const override { return m_WindowShouldClose; }
		void UpdateWindow() override;
		void PollEvents() override;
		void SwapBuffers() override;
		void Terminate() override;
		void TerminateOpenGLContext() override;

		// Win32
		void* Win32TryGetHWND() override;
	private:
		int m_WindowShouldClose;
		int m_Width, m_Height;
		TCHAR* m_ClassName; // LPCWSTR
		HINSTANCE m_HInstance;
		HWND m_HWnd;

		// OpenGL
		HDC m_DC;
		HGLRC m_RC;
		int m_PF;
		// could add a member variable as of OpenGLAttribs
	};
}