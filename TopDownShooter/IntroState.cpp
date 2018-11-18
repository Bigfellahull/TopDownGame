#include "stdafx.h"
#include "IntroState.h"
#include "PlayState.h"

using namespace DirectX;

extern void ExitGame();

void IntroState::Initialise(DX::DeviceResources const& deviceResources)
{

}

void IntroState::CleanUp()
{

}

void IntroState::Update(DX::StepTimer const& timer, Game* game)
{
	auto inputManager = game->GetInputManager();

	auto padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			auto playState = std::make_unique<PlayState>();
			game->ChangeCurrentState(std::move(playState));
		}

		if (padTracker.menu == GamePad::ButtonStateTracker::PRESSED)
		{
			ExitGame();
		}
	}

	auto keyboardTracker = inputManager->GetKeyboardTracker();
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		auto playState = std::make_unique<PlayState>();
		game->ChangeCurrentState(std::move(playState));
	}

	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Escape))
	{
		ExitGame();
	}
}

void IntroState::WindowSizeChanged(D3D11_VIEWPORT viewPort)
{
	
}

void IntroState::Render(DX::DeviceResources const& deviceResources)
{

}