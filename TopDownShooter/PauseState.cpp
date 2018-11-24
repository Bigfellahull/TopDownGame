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
	InputManager* inputManager = game->GetInputManager();

	GamePad::ButtonStateTracker& padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			game->PopState();
		}
	}

	Keyboard::KeyboardStateTracker& keyboardTracker = inputManager->GetKeyboardTracker();
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
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11RenderTargetView* renderTarget = deviceResources.GetRenderTargetView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Green);
}