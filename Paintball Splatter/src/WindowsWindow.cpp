#include "WindowsWindow.h"
#include "logger.h"
#include "InputSystem.h"
#include "Renderer.h"

#include <codecvt>

namespace Engine
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_CLOSE:
				PostQuitMessage(69);
				break;
			case WM_SIZE:
			{
				RECT rect;
				GetWindowRect(hWnd, &rect);
				int width, height;
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
				Renderer::SetViewport(width, height);
				//PS_LOGGER_LOG("resizing to: " << width << " " << height);
			}
				break;
			case WM_KEYDOWN:
				/*KeyDownEvent kde;
				kde.eventType = KeyboardEventType::KeyDown;
				kde.keyCode = WinKeyToInputSystem(wParam);
				EventManager::RegisterEvent(kde);*/
				//Input::RegisterKeyPress(KeyState::KeyDown, WinKeyToInputSystem(wParam));
				//PS_LOGGER_INFO(WinKeyToInputSystem(wParam));
				//CONVERT_INPUT_TO_NAME(WinKeyToInputSystem(wParam));
				//PS_LOGGER_ERROR(wParam);
				//if (wParam == VK_ESCAPE) PostQuitMessage(69);
				/*if (lParam & (1 << 30))
					Input::RegisterKeyPress(KeyState::KeyHeld, WinKeyToInputSystem(wParam)); //return 0;
				else
					Input::RegisterKeyPress(KeyState::KeyDown, WinKeyToInputSystem(wParam));*/
				if (lParam & (1 << 30))
					return 0;
				Input::RegisterKeyPress(KeyState::KeyDown, WinKeyToInputSystem(wParam));
				break;
			case WM_KEYUP:
				/*KeyUpEvent kue;
				kue.eventType = KeyboardEventType::KeyUp;
				kue.keyCode = WinKeyToInputSystem(wParam);
				EventManager::RegisterEvent(kue);*/
				//Input::RegisterKeyPress(KeyState::KeyUp, WinKeyToInputSystem(wParam));
				//PS_LOGGER_LOG(WinKeyToInputSystem(wParam));
				//CONVERT_INPUT_TO_NAME(WinKeyToInputSystem(wParam));
				Input::RegisterKeyPress(KeyState::KeyUp, WinKeyToInputSystem(wParam));
				break;
			case WM_LBUTTONDOWN: 
				PS_LOGGER_INFO(PS_MOUSE_LBUTTON);
				{
				POINTS pt = MAKEPOINTS(lParam);
				}
				break;
			case WM_LBUTTONUP:
				PS_LOGGER_INFO(PS_MOUSE_LBUTTON);
				break;

			case WM_RBUTTONDOWN:
				PS_LOGGER_INFO(PS_MOUSE_RBUTTON);
				break;
			case WM_RBUTTONUP:
				PS_LOGGER_INFO(PS_MOUSE_RBUTTON);
				break;

			case WM_MBUTTONDOWN:
				PS_LOGGER_INFO(PS_MOUSE_MBUTTON);
				break;
			case WM_MBUTTONUP:
				PS_LOGGER_INFO(PS_MOUSE_MBUTTON);
				break;

			case WM_LBUTTONDBLCLK:	// add CS_DBLCLKS
				PS_LOGGER_LOG(PS_MOUSE_LBUTTON);
				break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	WindowsWindow::WindowsWindow()
		: m_WindowShouldClose(0), m_Width(0), m_Height(0), m_ClassName(TEXT("Class Name")), m_HInstance(GetModuleHandle(nullptr)), m_HWnd(nullptr), m_DC(0), m_RC(0), m_PF(0)
	{
		//Create(0, 0, "T");
	}

	void WindowsWindow::Create(int width, int height, const std::string& title)
	{
		m_Width = width;
		m_Height = height;

		m_ClassName = TEXT("Class Name"); //L"Class Name";
		m_HInstance = GetModuleHandle(NULL); // supposedly shouldn't be ran in DLL

		// register windows class
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_HInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = m_ClassName; //L"title";
		wc.hIconSm = nullptr;
		PS_ASSERT(RegisterClassEx(&wc), "Failed to register window class.");

		// create windows instance
		m_HWnd = CreateWindowEx(0, m_ClassName, std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(title).c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW, 500, 200, 640, 480, nullptr, nullptr, m_HInstance, nullptr);

		PS_ASSERT(m_HWnd, "Failed to create window instance.");

		//PS_ASSERT(SystemParametersInfo(SPI_SETKEYBOARDDELAY, 1, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE), "Failed to disable key repeat.");
	}

	void WindowsWindow::CreateOpenGLContext()
	{
		//PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), }
		PIXELFORMATDESCRIPTOR pfd = {0};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);									// size of this pfd
		pfd.nVersion = 1;															// version number
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;	// support window, OpenGL, double buffered.
		pfd.iPixelType = PFD_TYPE_RGBA;												// RGBA type
		pfd.cColorBits = 24;														// 24-bit color depth
		pfd.cRedBits = 0;															// color bits ignored
		pfd.cRedShift = 0;
		pfd.cGreenBits = 0;
		pfd.cGreenShift = 0;
		pfd.cBlueBits = 0;
		pfd.cBlueShift = 0;
		pfd.cAlphaBits = 0;															// no alpha buffer
		pfd.cAlphaShift = 0;														// shift bit ignored
		pfd.cAccumBits = 0;															// no accumulation buffer
		pfd.cAccumRedBits = 0;														// accum bits ignored
		pfd.cAccumGreenBits = 0;
		pfd.cAccumBlueBits = 0;
		pfd.cAccumAlphaBits = 0;
		pfd.cDepthBits = 32;														// 32-bit z-buffer
		pfd.cStencilBits = 0;														// no stencil buffer
		pfd.cAuxBuffers = 0;														// no auxiliary buffer
		pfd.iLayerType = PFD_MAIN_PLANE;											// main layer
		pfd.bReserved = 0;															// reserved
		pfd.dwLayerMask = 0;														// layer masks ignored
		pfd.dwVisibleMask = 0;
		pfd.dwDamageMask = 0;

		m_DC = GetDC(m_HWnd);
		m_PF = ChoosePixelFormat(m_DC, &pfd);
		SetPixelFormat(m_DC, m_PF, &pfd);

		m_RC = wglCreateContext(m_DC); // replace
		wglMakeCurrent(m_DC, m_RC);
	}

	void WindowsWindow::CreateOpenGLExtensions(OpenGLAttribs* glAttribs)
	{
		if (glAttribs == nullptr)
			return; // no error, just so you can still use default values

		int attributes[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, glAttribs->GetWindowHint(WINDOW_OPENGL_VERSION_MAJOR),		//m_MajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, glAttribs->GetWindowHint(WINDOW_OPENGL_VERSION_MINOR),		//m_MinorVersion,
			WGL_CONTEXT_PROFILE_MASK_ARB, glAttribs->GetWindowHint(WINDOW_OPENGL_PROFILE),				//m_Profile,
			//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
			0
		};

		if (!wglewIsSupported("WGL_ARB_create_context"))
		{
			PS_ASSERT(false, "WGL_ARB_create_context not supported.");
		}

		m_RC = wglCreateContextAttribsARB(m_DC, 0, attributes);
		wglMakeCurrent(m_DC, m_RC);
	}

	void WindowsWindow::UpdateWindow()
	{
		ShowWindow(m_HWnd, SW_SHOW);
		::UpdateWindow(m_HWnd);
	}

	void WindowsWindow::PollEvents()
	{
		MSG msg;
		BOOL gResult;
		if ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //else if (!(GetMessage(&msg, nullptr, 0, 0) > 0))
		{
			m_WindowShouldClose = -1;
		}

		if (gResult == -1)
		{
			PS_LOGGER_FATAL("GetMessage encountered an error.");
			m_WindowShouldClose = -1;
		}
	}

	void WindowsWindow::SwapBuffers()
	{
		::SwapBuffers(m_DC);
	}

	void WindowsWindow::Terminate()
	{
		DestroyWindow(m_HWnd);
	}
	void WindowsWindow::TerminateOpenGLContext()
	{
		ReleaseDC(m_HWnd, m_DC);
		wglDeleteContext(m_RC);
	}
}