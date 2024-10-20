#include "Window.h"
#include "logger.h"
#ifdef PS_PLATFORM_WINDOWS
#include "WindowsWindow.h"
#endif

namespace Engine
{
	Window* Window::GenerateWindow()
	{
		IF_WINDOWS(return new WindowsWindow());

		PS_ASSERT(false, "Failed to create a window instance.");
		return nullptr;
	}
}