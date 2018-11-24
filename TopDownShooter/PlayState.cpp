#include "stdafx.h"

#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"
#include "PlayerShip.h"

#include "PositionComponent.h"
#include "MoveSystem.h"
#include "RenderSystem.h"

using namespace DirectX;

void PlayState::Initialise(DX::DeviceResources const& deviceResources)
{
	auto context = deviceResources.GetD3DDeviceContext();
	auto device = deviceResources.GetD3DDevice();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteBatch->SetViewport(deviceResources.GetScreenViewport());
    m_states = std::make_unique<CommonStates>(device);

	m_spriteFont = std::make_unique<SpriteFont>(device, L"Fonts\\SegoeUI_18.spritefont");

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Player.dds", nullptr, m_testTexture.ReleaseAndGetAddressOf())
	);

	m_manager = std::make_unique<Manager>();

	m_manager->CreateComponentStore<PositionComponent>();
	m_manager->CreateComponentStore<RenderComponent>();

	m_manager->AddSystem(System::Ptr(new SystemMove(*m_manager.get())));
	m_manager->AddSystem(System::Ptr(new SystemRender(*m_manager.get())));

	for (int i = 0; i < 5; ++i)
	{
		Entity ball = m_manager->CreateEntity();
		m_manager->AddComponent(ball, PositionComponent((0.0f + i) * 4, (0.0f + i) * 4));
		m_manager->AddComponent(ball, RenderComponent(*m_spriteBatch.get(), m_testTexture.Get()));
		m_manager->RegisterEntity(ball);
	}
}

void PlayState::CleanUp() 
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
    m_states.reset();
	m_testTexture.Reset();
	m_manager.reset();
}

void PlayState::Update(DX::StepTimer const& timer, Game* game)
{
	auto inputManager = game->GetInputManager();

#if _DEBUG
	swprintf_s(m_framesPerSecond, L"FPS %d\n", timer.GetFramesPerSecond());
#endif

	m_manager->UpdateEntities(timer);

	auto padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.menu == GamePad::ButtonStateTracker::PRESSED)
		{
			game->ChangeState(std::move(std::make_unique<IntroState>()));
		}

		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			game->PushState(std::move(std::make_unique<PauseState>()));
		}
	}

	auto keyboardTracker = inputManager->GetKeyboardTracker();
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::Escape))
	{
		game->ChangeState(std::move(std::make_unique<IntroState>()));
	}

	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::P))
	{
		game->PushState(std::move(std::make_unique<PauseState>()));
	}
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

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());

	m_manager->RenderEntities();

	m_spriteBatch->Draw(m_testTexture.Get(), XMFLOAT2(100, 100), nullptr, Colors::White);

	m_spriteBatch->End();

#if _DEBUG
    m_spriteBatch->Begin();
    m_spriteFont->DrawString(m_spriteBatch.get(), m_framesPerSecond, XMFLOAT2(10, 10), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
    m_spriteBatch->End();
#endif
};