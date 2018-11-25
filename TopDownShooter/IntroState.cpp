#include "stdafx.h"
#include "IntroState.h"
#include "PlayState.h"

using namespace DirectX;

void IntroState::Initialise(DX::DeviceResources const& deviceResources)
{

}

void IntroState::CleanUp()
{

}

void IntroState::Update(DX::StepTimer const& timer, Game* game)
{

}

void IntroState::HandleStateChange(Game* game)
{
	InputManager* inputManager = game->GetInputManager();

	GamePad::ButtonStateTracker& padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			game->ChangeState(std::move(std::make_unique<PlayState>()));
		}

		if (padTracker.back == GamePad::ButtonStateTracker::PRESSED)
		{
			game->Quit();
		}
	}

	Keyboard::KeyboardStateTracker& keyboardTracker = inputManager->GetKeyboardTracker();
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		game->ChangeState(std::move(std::make_unique<PlayState>()));
	}

	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Escape))
	{
		game->Quit();
	}
}

void IntroState::Pause()
{

}

void IntroState::Resume()
{

}

void IntroState::WindowSizeChanged(D3D11_VIEWPORT viewPort)
{
	
}

void IntroState::Render(DX::DeviceResources const& deviceResources)
{

}