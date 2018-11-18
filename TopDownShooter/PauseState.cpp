#include "stdafx.h"

#include "PauseState.h"
#include "PlayState.h"

using namespace DirectX;

void PauseState::Initialise(DX::DeviceResources const& deviceResources)
{

}

void PauseState::CleanUp()
{

}

void PauseState::Update(DX::StepTimer const& timer, Game* game)
{
	auto inputManager = game->GetInputManager();

	auto padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			game->PopState();
		}
	}

	auto keyboardTracker = inputManager->GetKeyboardTracker();
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::P))
	{
		game->PopState();
	}
}

void PauseState::Pause()
{

}

void PauseState::Resume()
{

}

void PauseState::WindowSizeChanged(D3D11_VIEWPORT viewPort)
{

}

void PauseState::Render(DX::DeviceResources const& deviceResources)
{
	auto context = deviceResources.GetD3DDeviceContext();
	auto renderTarget = deviceResources.GetRenderTargetView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Green);
}