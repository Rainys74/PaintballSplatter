#pragma once

// ------------Platform Specific------------
#ifdef PS_PLATFORM_WINDOWS // choose between IF_WINDOWS, CHECK_WINDOWS, WINDOWS_ONLY, WINDOWS_SPECIFIC, PLATFORM_WINDOWS
	#define IF_WINDOWS(x)	x
	#define IF_LINUX(x)
	#define IF_MACOS(x)
#elif defined(PS_PLATFORM_LINUX)
	#define IF_WINDOWS(x)
	#define IF_LINUX(x)		x
	#define IF_MACOS(x)
#elif defined(PS_PLATFORM_MACOS) // choose between MACOS or APPLE or maybe even MAC
	#define IF_WINDOWS(x)
	#define IF_LINUX(x)
	#define IF_MACOS(x)		x
#else
	#define IF_WINDOWS(x)
	#define IF_LINUX(x)
	#define IF_MACOS(x)
#endif
// -----------------------------------------

// ------------MACROS-----------------------

//#define INTERNAL_ASSERT(x, y) if (!(x)) (*Citrom::Logger::Severe() << __FILE__ << "(" << __LINE__ << "): " << "error: " << #y << " " << "line: " << #x); __debugbreak()
//#define ASSERT(x) INTERNAL_ASSERT(x, "");

#define _DEBUG_BREAK __debugbreak()

#define ASSERT(x) if (!(x)) {(*Engine::Logger::Severe() << __FILE__ << "(" << __LINE__ << "): " << "error: " << "" << " " << "line: " << #x) RESET_COLOR; _DEBUG_BREAK;}
#define PS_ASSERT(x, y) if (!(x)) {(*Engine::Logger::Severe() << __FILE__ << "(" << __LINE__ << "): " << "error: " << #y << " " << "line: " << #x) RESET_COLOR; _DEBUG_BREAK;}
// -----------------------------------------