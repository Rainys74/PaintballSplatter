#pragma once
#include "Core.h"
#include "OpenGLHints.h"

#include <string>

namespace Engine
{
	class Window
	{
	public:
		Window()
			: m_WindowShouldClose(0), m_Width(0), m_Height(0) {}
		virtual ~Window() {}

		static Window* GenerateWindow();

		virtual void Create(int width, int height, const std::string& title) = 0;
		virtual void CreateOpenGLContext() = 0;
		virtual void CreateOpenGLExtensions(OpenGLAttribs* glAttribs) = 0;
		virtual int WindowShouldClose() const { return m_WindowShouldClose; }
		virtual void UpdateWindow() = 0;
		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Terminate() = 0;
		virtual void TerminateOpenGLContext() = 0;

		// Win32
		virtual void* Win32TryGetHWND() = 0;
	private:
		int m_WindowShouldClose;
		int m_Width, m_Height;
	};
}