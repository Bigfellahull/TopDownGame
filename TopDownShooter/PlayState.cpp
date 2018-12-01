#include "stdafx.h"

#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"

#include "TranslationComponent.h"
#include "ProjectileSourceComponent.h"
#include "ProjectileComponent.h"
#include "RegionComponent.h"
#include "RenderComponent.h"
#include "FollowPlayerComponent.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "AvoidanceComponent.h"
#include "SeparationComponent.h"
#include "EnemyComponent.h"
#include "AvoidableComponent.h"

#include "MoveSystem.h"
#include "RenderSystem.h"
#include "ProjectileSourceSystem.h"
#include "ProjectileSystem.h"
#include "FollowPlayerSystem.h"
#include "ColliderSystem.h"
#include "AvoidanceSystem.h"
#include "DebugRenderSystem.h"
#include "SeparationSystem.h"
#include "EnemyActivatorSystem.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void PlayState::Initialise(DX::DeviceResources const& deviceResources)
{
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11Device1* device = deviceResources.GetD3DDevice();

	RECT windowSize = deviceResources.GetOutputSize();
	Rectangle screenBounds = Rectangle(0, 0, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top);

	// TODO: Improve this
	Rectangle worldBounds = Rectangle(0, 0, 4000, 4000);

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteBatch->SetViewport(deviceResources.GetScreenViewport());
    m_states = std::make_unique<CommonStates>(device);
	m_spriteFont = std::make_unique<SpriteFont>(device, L"Fonts\\SegoeUI_18.spritefont");
	m_assetManager = std::make_unique<AssetManager>(device);
	m_entityManager = std::make_unique<EntityManager>(worldBounds);

	m_entityManager->CreateComponentStore<TranslationComponent>();
	m_entityManager->CreateComponentStore<RenderComponent>();
	m_entityManager->CreateComponentStore<ProjectileSourceComponent>();
	m_entityManager->CreateComponentStore<ProjectileComponent>();
	m_entityManager->CreateComponentStore<RegionComponent>();
	m_entityManager->CreateComponentStore<FollowPlayerComponent>();
	m_entityManager->CreateComponentStore<ColliderComponent>();
    m_entityManager->CreateComponentStore<PlayerComponent>();
	m_entityManager->CreateComponentStore<AvoidanceComponent>();
	m_entityManager->CreateComponentStore<SeparationComponent>();
	m_entityManager->CreateComponentStore<EnemyComponent>();
	m_entityManager->CreateComponentStore<AvoidableComponent>();
		
	// The order systems are added in is important.
	// They are executed in order from first added to last.
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemEnemyActivator(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemProjectileSource(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemProjectile(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemFollowPlayer(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemAvoidance(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemSeparation(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemMove(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemCollider(*m_entityManager.get())));
	m_entityManager->AddSystem(std::shared_ptr<System>(new SystemRender(*m_entityManager.get())));
#if _DEBUG
    m_entityManager->AddSystem(std::shared_ptr<System>(new SystemDebugRender(*m_entityManager.get(), m_assetManager->GetTexture(DebugAsset))));
#endif

	m_camera = std::make_unique<Camera>(screenBounds);
	m_camera->SetLimits(worldBounds);
	
	m_regionEntity = m_entityManager->CreateEntity();
	m_entityManager->AddComponent(m_regionEntity, RegionComponent(Vector2(0, 0), Vector2(static_cast<float>(worldBounds.width), static_cast<float>(worldBounds.height))));
	m_entityManager->RegisterEntity(m_regionEntity);

    SpawnPlayer();

	m_enemyInverseSpawnChance = 60.0f;
}

void PlayState::SpawnPlayer()
{
    m_playerStatus.currentEntityId = m_entityManager->CreateEntity();
    m_entityManager->AddComponent(m_playerStatus.currentEntityId, TranslationComponent(GenerateRandomPosition(), Vector2::Zero, 0.0f));
    m_entityManager->AddComponent(m_playerStatus.currentEntityId, RenderComponent(*m_spriteBatch.get(), m_assetManager->GetTexture(PlayerAsset)));
    m_entityManager->AddComponent(m_playerStatus.currentEntityId, ProjectileSourceComponent(m_assetManager.get()));
    m_entityManager->AddComponent(m_playerStatus.currentEntityId, ColliderComponent(20.0f, 40.0f));
    m_entityManager->AddComponent(m_playerStatus.currentEntityId, PlayerComponent(&m_playerStatus));
    m_entityManager->RegisterEntity(m_playerStatus.currentEntityId);
    m_playerStatus.isAlive = true;
}

void PlayState::CleanUp() 
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
    m_states.reset();
	m_assetManager.reset();
	m_entityManager.reset();
}

void PlayState::Update(DX::StepTimer const& timer, Game* game)
{
#if _DEBUG
	swprintf_s(m_framesPerSecond, L"FPS %d\n", timer.GetFramesPerSecond());
	swprintf_s(m_entityCount, L"Entities: %zd\n", m_entityManager->GetNumberOfEntities());
#endif

	float dt = SlowModeEnabled ? 0.001f : static_cast<float>(timer.GetElapsedSeconds());

    if (m_playerStatus.isAlive) 
    {
        SpawnEnemies(dt);
    }

    UpdateUserInput(game->GetInputManager());

	m_entityManager->RebuildQuadTree();
    m_entityManager->UpdateEntities(dt);

	if (m_playerStatus.isAlive)
	{
		TranslationComponent& translation = m_entityManager->GetComponentStore<TranslationComponent>().Get(m_playerStatus.currentEntityId);
		m_camera->LookAt(translation.position);
	}
}

void PlayState::UpdateUserInput(InputManager* inputManager)
{
    GamePad::State state = inputManager->GetGamePadState();
    Keyboard::State kb = inputManager->GetKeyboardState();

    if (m_playerStatus.isAlive) 
    {
        TranslationComponent& translation = m_entityManager->GetComponentStore<TranslationComponent>().Get(m_playerStatus.currentEntityId);
        ProjectileSourceComponent& projectile = m_entityManager->GetComponentStore<ProjectileSourceComponent>().Get(m_playerStatus.currentEntityId);

        projectile.aimDirection = Vector2{ state.thumbSticks.rightX, state.thumbSticks.rightY };
        projectile.aimDirection.y *= -1;

        Vector2 acceleration{ state.thumbSticks.leftX, state.thumbSticks.leftY };
        if (state.IsConnected())
        {
            acceleration.y *= -1;
        }

        if (kb.Up)
        {
            projectile.aimDirection.y = -1.0f;
        }
        if (kb.Right)
        {
            projectile.aimDirection.x = 1.0f;
        }
        if (kb.Down)
        {
            projectile.aimDirection.y = 1.0f;
        }
        if (kb.Left)
        {
            projectile.aimDirection.x = -1.0f;
        }
        if (kb.W)
        {
            acceleration.y = -1.0f;
        }
        if (kb.D)
        {
            acceleration.x = 1.0f;
        }
        if (kb.S)
        {
            acceleration.y = 1.0f;
        }
        if (kb.A)
        {
            acceleration.x = -1.0f;
        }

        float movementSpeed = 8000.0f;
        float drag = 10.0f;

        float accelerationLength = acceleration.LengthSquared();
        if (accelerationLength > 1.0f)
        {
            acceleration *= (1.0f / sqrt(accelerationLength));
        }

        translation.acceleration = (acceleration * movementSpeed) + (translation.velocity * -drag);
    }
    else if (kb.Q || state.IsBPressed())
    {
        SpawnPlayer();
    }
}

Vector2 PlayState::GenerateRandomPosition()
{
	RegionComponent& region = m_entityManager->GetComponentStore<RegionComponent>().Get(m_regionEntity);

	float xBounds = static_cast<float>(region.max.x) - 20.0f;
	float yBounds = static_cast<float>(region.max.y) - 20.0f;

	return Vector2(MathHelper::Random(0.0f, xBounds), MathHelper::Random(0.0f, yBounds));
}

void PlayState::SpawnEnemies(float dt)
{
	if (MathHelper::Random(0, static_cast<int>(m_enemyInverseSpawnChance)) == 0)
	{
		TranslationComponent& playerTranslation = m_entityManager->GetComponentStore<TranslationComponent>().Get(m_playerStatus.currentEntityId);

		Vector2 spawnPosition = Vector2::Zero;
		int positionChecks = 0;
		do
		{
			spawnPosition = GenerateRandomPosition();
			positionChecks++;
		} while ((Vector2::DistanceSquared(spawnPosition, playerTranslation.position) < std::pow(100.0f, 2)) || positionChecks < 10);

		auto enemy = m_entityManager->CreateEntity();
		m_entityManager->AddComponent(enemy, TranslationComponent(spawnPosition, Vector2(0, 0), MathHelper::Random(0.0f, 6.2f)));
		m_entityManager->AddComponent(enemy, RenderComponent(*m_spriteBatch.get(), m_assetManager->GetTexture(SeekerEnemyAsset)));
		m_entityManager->AddComponent(enemy, EnemyComponent(1.0f));
		m_entityManager->AddComponent(enemy, FollowPlayerComponent(&m_playerStatus, 6500.0f, 15.0f));
		m_entityManager->AddComponent(enemy, AvoidanceComponent());
		m_entityManager->AddComponent(enemy, SeparationComponent());
		m_entityManager->AddComponent(enemy, ColliderComponent(15.0f, 35.0f));
		m_entityManager->RegisterEntity(enemy);
	}

	if (m_enemyInverseSpawnChance > 1.0f)
	{
		m_enemyInverseSpawnChance -= dt;
		m_enemyInverseSpawnChance = std::max(1.0f, m_enemyInverseSpawnChance);
	}
}

void PlayState::HandleStateChange(Game* game)
{
	InputManager* inputManager = game->GetInputManager();

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
	
	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Black);

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Texture, m_states->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, m_camera->GetViewMatrix());

	m_entityManager->RenderEntities();

	m_spriteBatch->End();

#if _DEBUG
    m_spriteBatch->Begin();
    m_spriteFont->DrawString(m_spriteBatch.get(), m_framesPerSecond, XMFLOAT2(10, 10), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
	m_spriteFont->DrawString(m_spriteBatch.get(), m_entityCount, XMFLOAT2(10, 30), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
    m_spriteBatch->End();
#endif
};