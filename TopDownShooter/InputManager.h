#pragma once

#include "GamePad.h"
#include "Keyboard.h"

class InputManager
{
public:
	InputManager();

	void Update();

	DirectX::GamePad::State GetGamePadState();
	DirectX::GamePad::ButtonStateTracker& GetGamePadTracker();

	DirectX::Keyboard::State GetKeyboardState();
	DirectX::Keyboard::KeyboardStateTracker& GetKeyboardTracker();
private:
	std::unique_ptr<DirectX::GamePad> m_gamePad;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::GamePad::ButtonStateTracker> m_gamePadTracker;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
};
