#pragma once

using KeyCode = int;

#define PS_KEY_NULL	   -1

#define PS_KEY_A		1
#define PS_KEY_B		2
#define PS_KEY_C		3
#define PS_KEY_D		4
#define PS_KEY_E		5
#define PS_KEY_F		6
#define PS_KEY_G		7
#define PS_KEY_H		8
#define PS_KEY_I		9
#define PS_KEY_J		10
#define PS_KEY_K		11
#define PS_KEY_L		12
#define PS_KEY_M		13
#define PS_KEY_N		14
#define PS_KEY_O		15
#define PS_KEY_P		16
#define PS_KEY_Q		17
#define PS_KEY_R		18
#define PS_KEY_S		19
#define PS_KEY_T		20
#define PS_KEY_U		21
#define PS_KEY_V		22
#define PS_KEY_W		23
#define PS_KEY_X		24
#define PS_KEY_Y		25
#define PS_KEY_Z		26

#define PS_KEY_BACKSPACE	27
#define PS_KEY_TAB			28
#define PS_KEY_RETURN		29 // enter
#define PS_KEY_LSHIFT		30
#define PS_KEY_RSHIFT		31
#define PS_KEY_CTRL			32
#define PS_KEY_LALT			33
#define PS_KEY_RALT			34
#define PS_KEY_PAUSE		35
#define PS_KEY_CAPSLOCK		36
#define PS_KEY_ESCAPE		37
#define PS_KEY_SPACE		38
#define PS_KEY_PGUP			39
#define PS_KEY_PGDN			40
#define PS_KEY_END			41
#define PS_KEY_HOME			42
#define PS_KEY_UP			43
#define PS_KEY_LEFT			44
#define PS_KEY_DOWN			45
#define PS_KEY_RIGHT		46
#define PS_KEY_PRTSC		47
#define PS_KEY_INSERT		48
#define PS_KEY_DELETE		49

#define PS_KEY_NUMPAD0		50
#define PS_KEY_NUMPAD1		51
#define PS_KEY_NUMPAD2		52
#define PS_KEY_NUMPAD3		53
#define PS_KEY_NUMPAD4		54
#define PS_KEY_NUMPAD5		55
#define PS_KEY_NUMPAD6		56
#define PS_KEY_NUMPAD7		57
#define PS_KEY_NUMPAD8		58
#define PS_KEY_NUMPAD9		59
#define PS_KEY_MULTIPLY		60	// numpad *
#define PS_KEY_ADD			61	// numpad +
#define PS_KEY_SUBTRACT		62  // numpad -
#define PS_KEY_DECIMAL		63	// numpad .
#define PS_KEY_DIVIDE		64	// numpad /
#define PS_KEY_NUMLOCK		65

#define PS_KEY_F1			66
#define PS_KEY_F2			67
#define PS_KEY_F3			68
#define PS_KEY_F4			69
#define PS_KEY_F5			70
#define PS_KEY_F6			71
#define PS_KEY_F7			72
#define PS_KEY_F8			73
#define PS_KEY_F9			74
#define PS_KEY_F10			75
#define PS_KEY_F11			76
#define PS_KEY_F12			77	// should go up to 24

#define PS_KEY_SCRLK		78

#define PS_KEY_ALPHA1		79
#define PS_KEY_ALPHA2		80
#define PS_KEY_ALPHA3		81
#define PS_KEY_ALPHA4		82
#define PS_KEY_ALPHA5		83
#define PS_KEY_ALPHA6		84
#define PS_KEY_ALPHA7		85
#define PS_KEY_ALPHA8		86
#define PS_KEY_ALPHA9		87
#define PS_KEY_ALPHA0		88

// ------Forgotten additional------
#define PS_KEY_LCTRL		89
#define PS_KEY_RCTRL		90

#define PS_KEY_BACKQUOTE	91	// backtick, tilde or that console key `
#define PS_KEY_ALPHAMINUS	92	// alpha -
#define PS_KEY_ALPHAEQUAL	93	// alpha =
#define PS_KEY_SLASH		94
#define PS_KEY_BACKSLASH	95
#define PS_KEY_APPS			96	// Applications key, Menu key, key that looks like a cupboard

#define PS_KEY_LEFTBRACKET	97
#define PS_KEY_RIGHTBRACKET 98
#define PS_KEY_SEMICOLON	99
#define PS_KEY_APOSTROPHE	100 // quote '
#define PS_KEY_COMMA		101
#define PS_KEY_PERIOD		102
#define PS_KEY_START		103 // Windows - Windows, Super - Linux, Command - MacOS
// --------------------------------

#define PS_MOUSE_NULL		-1

#define PS_MOUSE_LBUTTON	0
#define PS_MOUSE_RBUTTON	1
#define PS_MOUSE_MBUTTON	2

#ifdef PS_PLATFORM_WINDOWS
#include <Windows.h>
namespace Engine 
{
	int WinKeyToInputSystem(WPARAM key);
	int WinMouseToInputSystem(WPARAM button);
}
#endif

#ifdef PS_DEBUG
#define CONVERT_INPUT_TO_NAME(x) PrintInputNumberToName(x)
#elif defined(PS_RELEASE)
#define CONVERT_INPUT_TO_NAME(x)
#endif
void PrintInputNumberToName(int number);