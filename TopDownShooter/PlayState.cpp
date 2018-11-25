#include "stdafx.h"

#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"
#include "TranslationComponent.h"
#include "ProjectileSourceComponent.h"
#include "ProjectileComponent.h"
#include "RegionComponent.h"
#include "RenderComponent.h"
#include "MoveSystem.h"
#include "RenderSystem.h"
#include "ProjectileSourceSystem.h"
#include "ProjectileSystem.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void PlayState::Initialise(DX::DeviceResources const& deviceResources)
{
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11Device1* device = deviceResources.GetD3DDevice();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteBatch->SetViewport(deviceResources.GetScreenViewport());
    m_states = std::make_unique<CommonStates>(device);

	m_spriteFont = std::make_unique<SpriteFont>(device, L"Fonts\\SegoeUI_18.spritefont");

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Player.dds", m_testResource.ReleaseAndGetAddressOf(), m_testTexture.ReleaseAndGetAddressOf())
	);

	m_manager = std::make_unique<EntityManager>();

	m_manager->CreateComponentStore<TranslationComponent>();
	m_manager->CreateComponentStore<RenderComponent>();
	m_manager->CreateComponentStore<ProjectileSourceComponent>();
	m_manager->CreateComponentStore<ProjectileComponent>();
	m_manager->CreateComponentStore<RegionComponent>();

	m_manager->AddSystem(std::shared_ptr<System>(new SystemMove(*m_manager.get())));
	m_manager->AddSystem(std::shared_ptr<System>(new SystemRender(*m_manager.get())));
	m_manager->AddSystem(std::shared_ptr<System>(new SystemProjectileSource(*m_manager.get())));
	m_manager->AddSystem(std::shared_ptr<System>(new SystemProjectile(*m_manager.get())));

	m_regionEntity = m_manager->CreateEntity();
	RECT windowSize = deviceResources.GetOutputSize();
	m_manager->AddComponent(m_regionEntity, RegionComponent(Vector2(0, 0), 
		Vector2(static_cast<float>(windowSize.right - windowSize.left), static_cast<float>(windowSize.bottom - windowSize.top))));
	m_manager->RegisterEntity(m_regionEntity);
	
	m_playerEntity = m_manager->CreateEntity();
	m_manager->AddComponent(m_playerEntity, TranslationComponent(Vector2(100, 100), Vector2(0, 0), 0.0f));
	m_manager->AddComponent(m_playerEntity, RenderComponent(*m_spriteBatch.get(), m_testTexture.Get(), m_testResource.Get()));
	m_manager->AddComponent(m_playerEntity, ProjectileSourceComponent());
	m_manager->RegisterEntity(m_playerEntity);
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
	InputManager* inputManager = game->GetInputManager();

#if _DEBUG
	swprintf_s(m_framesPerSecond, L"FPS %d\n", timer.GetFramesPerSecond());
	swprintf_s(m_entityCount, L"Entities: %d\n", m_manager->GetNumberOfEntities());
#endif

	GamePad::State state = inputManager->GetGamePadState();
	TranslationComponent& translation = m_manager->GetComponentStore<TranslationComponent>().Get(m_playerEntity);
	ProjectileSourceComponent& projectile = m_manager->GetComponentStore<ProjectileSourceComponent>().Get(m_playerEntity);

	projectile.aimDirection = Vector2{ state.thumbSticks.rightX, state.thumbSticks.rightY };
	projectile.aimDirection.y *= -1;

	Vector2 acceleration{ state.thumbSticks.leftX, state.thumbSticks.leftY };

	if (state.IsConnected())
	{
		acceleration.y *= -1;
	}

	Keyboard::State kb = inputManager->GetKeyboardState();
	if (kb.Up)
	{
		acceleration.y = -1.0f;
	}
	if (kb.Right)
	{
		acceleration.x = 1.0f;
	}
	if (kb.Down)
	{
		acceleration.y = 1.0f;
	}
	if (kb.Left)
	{
		acceleration.x = -1.0f;
	}

	// TODO: Work out units and why this has to be so high
	float movementSpeed = 8000.0f;
	float drag = 10.0f;

	// Ensure moving diagonally isn't faster than usual.
	float accLength = acceleration.LengthSquared();
	if (accLength > 1.0f)
	{
		acceleration *= (1.0f / sqrt(accLength));
	}

	translation.acceleration = (acceleration * movementSpeed) + (translation.velocity * -drag);

	m_manager->UpdateEntities(timer);

	GamePad::ButtonStateTracker& padTracker = inputManager->GetGamePadTracker();
	if (inputManager->GetGamePadState().IsConnected())
	{
		if (padTracker.back == GamePad::ButtonStateTracker::PRESSED)
		{
			game->ChangeState(std::move(std::make_unique<IntroState>()));
		}

		if (padTracker.start == GamePad::ButtonStateTracker::PRESSED)
		{
			game->PushState(std::move(std::make_unique<PauseState>()));
		}
	}

	Keyboard::KeyboardStateTracker& keyboardTracker = inputManager->GetKeyboardTracker();
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
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11RenderTargetView* renderTarget = deviceResources.GetRenderTargetView();
	
	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Red);

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());

	m_manager->RenderEntities();

	m_spriteBatch->End();

#if _DEBUG
    m_spriteBatch->Begin();
    m_spriteFont->DrawString(m_spriteBatch.get(), m_framesPerSecond, XMFLOAT2(10, 10), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
	m_spriteFont->DrawString(m_spriteBatch.get(), m_entityCount, XMFLOAT2(10, 30), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
    m_spriteBatch->End();
#endif
};