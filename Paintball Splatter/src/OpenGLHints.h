#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>

using Attribute = int;

#define WINDOW_OPENGL_V_NONE					-1

#define WINDOW_OPENGL_PROFILE					0
#define WINDOW_OPENGL_VERSION_MAJOR				1
#define WINDOW_OPENGL_VERSION_MINOR				2

#define WINDOW_OPENGL_V_CORE_PROFILE			WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define WINDOW_OPENGL_V_COMPATIBILITY_PROFILE	WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB

class OpenGLAttribs
{
public:
	OpenGLAttribs()
		: m_Profile(WINDOW_OPENGL_V_CORE_PROFILE), m_MajorVersion(4), m_MinorVersion(6) {}

	void SetWindowHint(int glAttribute, int value)
	{
		switch (glAttribute)
		{
			case WINDOW_OPENGL_PROFILE:
				//if (value != WINDOW_OPENGL_V_COMPATIBILITY_PROFILE)
					m_Profile = value;
				break;
			case WINDOW_OPENGL_VERSION_MAJOR:
				m_MajorVersion = value;
				break;
			case WINDOW_OPENGL_VERSION_MINOR:
				m_MinorVersion = value;
				break;
		}
	}
	int GetWindowHint(int glAttribute)
	{
		#define GET(x, y)	case x:\
								return y;\
								break;

		/*switch (glAttribute)
		{
		case WINDOW_OPENGL_PROFILE:
			return m_Profile;
			break;
		case WINDOW_OPENGL_VERSION_MAJOR:
			return m_MajorVersion;
			break;
		case WINDOW_OPENGL_VERSION_MINOR:
			return m_MinorVersion;
			break;
		}*/
		switch (glAttribute)
		{
			GET(WINDOW_OPENGL_PROFILE, m_Profile);
			GET(WINDOW_OPENGL_VERSION_MAJOR, m_MajorVersion);
			GET(WINDOW_OPENGL_VERSION_MINOR, m_MinorVersion);
		}
		return WINDOW_OPENGL_V_NONE;
	}

private:
	Attribute m_Profile;
	Attribute m_MajorVersion;
	Attribute m_MinorVersion;
};