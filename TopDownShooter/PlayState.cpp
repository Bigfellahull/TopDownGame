#include "stdafx.h"

#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"

using namespace DirectX;

void PlayState::Initialise(DX::DeviceResources const& deviceResources)
{
	auto context = deviceResources.GetD3DDeviceContext();
	auto device = deviceResources.GetD3DDevice();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteBatch->SetViewport(deviceResources.GetScreenViewport());

	m_spriteFont = std::make_unique<SpriteFont>(device, L"Fonts\\SegoeUI_18.spritefont");
}

void PlayState::CleanUp() 
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
}

void PlayState::Update(DX::StepTimer const& timer, Game* game)
{
	auto inputManager = game->GetInputManager();

	auto padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.menu == GamePad::ButtonStateTracker::PRESSED)
		{
			auto introState = std::make_unique<IntroState>();
			game->ChangeState(std::move(introState));
		}

		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			auto pauseState = std::make_unique<PauseState>();
			game->PushState(std::move(pauseState));
		}
	}

	auto keyboardTracker = inputManager->GetKeyboardTracker();
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Escape))
	{
		auto introState = std::make_unique<IntroState>();
		game->ChangeState(std::move(introState));
	}

	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::P))
	{
		auto pauseState = std::make_unique<PauseState>();
		game->PushState(std::move(pauseState));
	}

#if _DEBUG
	swprintf_s(m_framesPerSecond, L"FPS %d\n", timer.GetFramesPerSecond());
#endif
}

void PlayState::Pause()
{

}

void PlayState::Resume()
{

}

void PlayState::WindowSizeChanged(D3D11_VIEWPORT viewPort)
{
	m_spriteBatch->SetViewport(viewPort);
}

void PlayState::Render(DX::DeviceResources const& deviceResources)
{
	auto context = deviceResources.GetD3DDeviceContext();
	auto renderTarget = deviceResources.GetRenderTargetView();
	
	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Red);

	m_spriteBatch->Begin();

#if _DEBUG
	m_spriteFont->DrawString(m_spriteBatch.get(), m_framesPerSecond, XMFLOAT2(10, 10), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
#endif

	m_spriteBatch->End();
}