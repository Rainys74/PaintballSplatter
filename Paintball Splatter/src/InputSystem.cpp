#include "InputSystem.h"
#include "logger.h"

KeyCode Input::m_LastKeyPressed = 0;
KeyState Input::m_LastKeyState;
bool Input::m_KeyHeld;

void Input::RegisterKeyPress(KeyState keyState, KeyCode keyCode)
{
	m_LastKeyState = keyState;
	m_LastKeyPressed = keyCode;
}

bool Input::GetKeyPress(KeyState keyState, KeyCode keyCode)
{
	if (m_LastKeyState == KeyState::KeyDown)
		m_KeyHeld = true;

	if (m_LastKeyState == KeyState::KeyUp)
		m_KeyHeld = false;

	if (keyState == KeyState::KeyHeld && keyCode == m_LastKeyPressed)
	{
		return 0;
	}
	else if (keyState == m_LastKeyState && keyCode == m_LastKeyPressed)
	{
		/*if (m_KeyHeld)
			m_LastKeyState = KeyState::KeyHeld;*/
		return true;
	}
	else 
	{
		/*if (m_KeyHeld)
			m_LastKeyState = KeyState::KeyHeld;*/
		return false;
	}

	PS_ASSERT(false, "Failed to get key press.");
	return false;
}

void Input::ClearKeyPresses()
{
	//m_LastKeyState = KeyState::None;
	if (m_LastKeyState == KeyState::KeyHeld)
		return;
	m_LastKeyPressed = PS_KEY_NULL;
}

void Input::Update()
{
}