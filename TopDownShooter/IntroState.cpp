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
	auto inputManager = game->GetInputManager();

	auto padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			auto playState = std::make_unique<PlayState>();
			game->ChangeState(std::move(playState));
		}

		if (padTracker.menu == GamePad::ButtonStateTracker::PRESSED)
		{
			game->Quit();
		}
	}

	auto keyboardTracker = inputManager->GetKeyboardTracker();
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		auto playState = std::make_unique<PlayState>();
		game->ChangeState(std::move(playState));
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