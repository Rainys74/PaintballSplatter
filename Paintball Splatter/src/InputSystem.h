#pragma once
#include "InputData.h"

enum class KeyState
{
	None = 0,
	KeyDown,
	KeyUp, KeyHeld
};

class Input
{
public:
	static void RegisterKeyPress(KeyState keyState, KeyCode keyCode);
	static bool GetKeyPress(KeyState keyState, KeyCode keyCode);
	static void ClearKeyPresses();
	static void Update();
private:
	static KeyCode m_LastKeyPressed;
	static KeyState m_LastKeyState;
	static bool m_KeyHeld;
};