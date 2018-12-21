#include "stdafx.h"

#include "GameOverState.h"
#include "PlayState.h"

using namespace DirectX;

void GameOverState::Initialise(DX::DeviceResources const& deviceResources)
{
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11Device1* device = deviceResources.GetD3DDevice();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteBatch->SetViewport(deviceResources.GetScreenViewport());
	m_spriteFont = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");
}

void GameOverState::CleanUp()
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
}

void GameOverState::Update(DX::StepTimer const& timer, Game* game)
{

}

void GameOverState::HandleStateChange(Game* game)
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
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		game->PopState();
	}
}

void GameOverState::Pause()
{

}

void GameOverState::Resume(std::string previousState)
{

}

void GameOverState::WindowSizeChanged(D3D11_VIEWPORT viewPort)
{

}

void GameOverState::Render(DX::DeviceResources const& deviceResources)
{
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11RenderTargetView* renderTarget = deviceResources.GetRenderTargetView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Black);
	
	D3D11_VIEWPORT viewPort = deviceResources.GetScreenViewport();

	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), L"GAME OVER", XMFLOAT2((viewPort.Width / 2) - 80, (viewPort.Height / 2) - 80), Colors::White, 0.0f, XMFLOAT2(0, 0), 1.1f);
	m_spriteBatch->End();
}