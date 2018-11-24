#include "stdafx.h"

#include "InputManager.h"

using namespace DirectX;

InputManager::InputManager()
{
	m_gamePad = std::make_unique<GamePad>();
	m_gamePadTracker = std::make_unique<GamePad::ButtonStateTracker>();
	
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
}

void InputManager::Update()
{
	GamePad::State gamePadState = m_gamePad->GetState(0);

	if (gamePadState.IsConnected())
	{
		m_gamePadTracker->Update(gamePadState);
	}

	Keyboard::State keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(keyboardState);
}

const GamePad::State InputManager::GetGamePadState()
{
	return m_gamePad->GetState(0);
}

GamePad::ButtonStateTracker& InputManager::GetGamePadTracker()
{
	return *m_gamePadTracker;
}

const Keyboard::State InputManager::GetKeyboardState()
{
	return m_keyboard->GetState();
}

Keyboard::KeyboardStateTracker& InputManager::GetKeyboardTracker()
{
	return *m_keyboardTracker;
}