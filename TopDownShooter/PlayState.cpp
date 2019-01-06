#include "stdafx.h"

#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"
#include "GameOverState.h"

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
#include "WanderComponent.h"
#include "DestructableComponent.h"
#include "ExhaustPlumeComponent.h"
#include "HealthComponent.h"

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
#include "WanderSystem.h"
#include "DestructableSystem.h"
#include "ExhaustPlumeSystem.h"
#include "CollisionHandlerSystem.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void PlayState::Initialise(DX::DeviceResources const& deviceResources)
{
	ID3D11DeviceContext1* context = deviceResources.GetD3DDeviceContext();
	ID3D11Device1* device = deviceResources.GetD3DDevice();

	RECT windowSize = deviceResources.GetOutputSize();
	Rectangle screenBounds = Rectangle(0, 0, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top);

	// TODO: Improve this
	Rectangle worldBounds = Rectangle(0, 0, 1500, 1500);

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteBatch->SetViewport(deviceResources.GetScreenViewport());
    m_states = std::make_unique<CommonStates>(device);
	m_spriteFont = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");
	m_assetManager = std::make_unique<AssetManager>(device);
	m_entityManager = std::make_unique<EntityManager>(worldBounds);
	m_particleManager = std::make_unique<ParticleManager>(worldBounds);

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
	m_entityManager->CreateComponentStore<WanderComponent>();
	m_entityManager->CreateComponentStore<DestructableComponent>();
	m_entityManager->CreateComponentStore<ExhaustPlumeComponent>();
	m_entityManager->CreateComponentStore<HealthComponent>();
		
	// The order systems are added in is important.
	// They are executed in order from first added to last.
	m_entityManager->AddSystem(std::make_shared<SystemEnemyActivator>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemProjectileSource>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemProjectile>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemFollowPlayer>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemWander>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemAvoidance>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemSeparation>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemMove>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemCollider>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemCollisionHandler>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemExhaustPlume>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemDestructable>(*m_entityManager.get()));
	m_entityManager->AddSystem(std::make_shared<SystemRender>(*m_entityManager.get()));
#if _DEBUG
	if (ShowDebugInformation)
	{
		m_entityManager->AddSystem(std::make_shared<SystemDebugRender>(*m_entityManager.get(), m_assetManager->GetTexture(DebugAsset)));
	}
#endif

	m_camera = std::make_unique<Camera>(screenBounds);
	Rectangle cameraLimit = worldBounds;
	cameraLimit.x -= 50;
	cameraLimit.y -= 50;
	cameraLimit.width += 100;
	cameraLimit.height += 100;
	m_camera->SetLimits(cameraLimit);
	//m_camera->SetZoom(0.5f);
	
	m_regionEntity = m_entityManager->CreateEntity();
	m_entityManager->AddComponent(m_regionEntity, RegionComponent(Vector2(0, 0), Vector2(static_cast<float>(worldBounds.width), static_cast<float>(worldBounds.height))));
	m_entityManager->RegisterEntity(m_regionEntity);

    SpawnPlayer(true);

	m_fixedBackground = m_assetManager->GetTexture(BackgroundLayer1);
	
	m_backgroundBuffer = std::make_unique<BackgroundBuffer>();
	D3D11_BUFFER_DESC cbufferDesc = {};
	cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbufferDesc.ByteWidth = (sizeof(BackgroundBuffer) + 15) / 16 * 16;
	DX::ThrowIfFailed(
		device->CreateBuffer(&cbufferDesc, nullptr, m_backgroundD3dBuffer.ReleaseAndGetAddressOf())
	);

	std::vector<uint8_t> backgroundPsBlob = DX::ReadData(L"BackgroundPixelShader.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(&backgroundPsBlob.front(), backgroundPsBlob.size(), nullptr, &m_backgroundPs)
	);

	std::vector<uint8_t> backgroundVsBlob = DX::ReadData(L"BackgroundVertexShader.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(&backgroundVsBlob.front(), backgroundVsBlob.size(), nullptr, &m_backgroundVs)
	);
	
	m_backgroundLayers.push_back(std::make_unique<BackgroundLayer>(m_camera.get(), Vector2(0.4f, 0.4f)));
	m_backgroundLayers.push_back(std::make_unique<BackgroundLayer>(m_camera.get(), Vector2(0.8f, 0.8f)));
	
	m_backgroundLayers[0]->AddBackgroundSprite(BackgroundSprite(Vector2(1000.0f, 500.0f), m_assetManager->GetTexture(BackgroundLayer2), 0.5f));
	m_backgroundLayers[0]->AddBackgroundSprite(BackgroundSprite(Vector2(300.0f, 200.0f), m_assetManager->GetTexture(BackgroundLayer3), 0.4f));
	m_backgroundLayers[1]->AddBackgroundSprite(BackgroundSprite(Vector2(600.0f, 1200.0f), m_assetManager->GetTexture(BackgroundLayer4), 0.5f));
}

// TODO: Improve this!
void PlayState::RestartGame()
{
	m_enemyInverseSpawnChance = 120.0f;
	ComponentStore<DestructableComponent>& destructableComponents = m_entityManager->GetComponentStore<DestructableComponent>();
	for (auto& x : destructableComponents.GetComponents())
	{
		m_entityManager->QueueEntityForDrop(x.first);
	}
	m_entityManager->DropEntities();
	m_particleManager->ClearParticles();
	SpawnPlayer(true);
}

void PlayState::SpawnPlayer(bool reset)
{
	Entity playerEntity = m_entityManager->CreateEntity();

	if (reset)
	{
		m_playerStatus.Reset(playerEntity);
	}
	else
	{
		m_playerStatus.SetCurrentEntityId(playerEntity);
	}
	
	Vector2 position = GenerateRandomPosition();
    m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), TranslationComponent(position, Vector2::Zero, 0.0f));
    m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), RenderComponent(*m_spriteBatch.get(), m_assetManager->GetTexture(PlayerAsset), m_spriteFont.get()));
    m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), ProjectileSourceComponent(m_assetManager.get(), m_particleManager.get()));
    m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), ColliderComponent(20.0f, 40.0f));
    m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), PlayerComponent(&m_playerStatus));
	m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), DestructableComponent(m_particleManager.get(), m_assetManager->GetTexture(ParticleAsset), 900.0f, 300));
	m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), ExhaustPlumeComponent(m_particleManager.get(), m_assetManager->GetTexture(ParticleAsset), m_assetManager->GetTexture(ParticleGlowAsset)));
	m_entityManager->AddComponent(m_playerStatus.GetCurrentEntityId(), HealthComponent(30.0f));
	m_entityManager->RegisterEntity(m_playerStatus.GetCurrentEntityId());

	m_camera->LookAt(position);
}

void PlayState::CleanUp() 
{
	m_states.reset();
	m_spriteBatch.reset();
	m_spriteFont.reset();
    
	m_camera.reset();
	m_assetManager.reset();
	m_entityManager.reset();
	
	m_backgroundLayers.clear();
	m_backgroundBuffer.reset();
	m_backgroundD3dBuffer.Reset();

	m_backgroundPs.Reset();
	m_backgroundVs.Reset();
}

void PlayState::Update(DX::StepTimer const& timer, Game* game)
{
	float dt = SlowModeEnabled ? 0.001f : static_cast<float>(timer.GetElapsedSeconds());

	swprintf_s(m_scoreDisplay, L"Score: %d\n", m_playerStatus.GetScore());
	swprintf_s(m_multiplierDisplay, L"Multiplier: %d\n", m_playerStatus.GetMultiplier());
	// TODO: Fix this! Maybe player status should be a component and fold into entity system?
	int shieldValue = 0;
	if (m_playerStatus.IsAlive())
	{
		HealthComponent& playerHealth = m_entityManager->GetComponentStore<HealthComponent>().Get(m_playerStatus.GetCurrentEntityId());
		shieldValue = static_cast<int>((playerHealth.hitPoints / 30.0f) * 100.0f);
	}
	swprintf_s(m_healthDisplay, L"Shields: %d%%\n", shieldValue);
	swprintf_s(m_livesDisplay, L"Lives: %d\n", m_playerStatus.GetLives());

    if (m_playerStatus.IsAlive() && m_entityManager->GetNumberOfEntities() < 200) 
    {
        SpawnEnemies(dt);
    }

    UpdateUserInput(game->GetInputManager());

	m_entityManager->RebuildQuadTree();
    m_entityManager->UpdateEntities(dt, static_cast<float>(timer.GetTotalSeconds()));
	// Ensure we update particles after entities as particles get 
	// added in the entity systems.
	m_particleManager->Update(dt);

	if (m_playerStatus.IsAlive())
	{
		TranslationComponent& translation = m_entityManager->GetComponentStore<TranslationComponent>().Get(m_playerStatus.GetCurrentEntityId());
		m_camera->LookAtSmooth(translation.position, dt);

		m_playerStatus.Update(dt);
	}

#if _DEBUG
	swprintf_s(m_framesPerSecond, L"FPS %d\n", timer.GetFramesPerSecond());
	swprintf_s(m_entityCount, L"Entities: %zd\n", m_entityManager->GetNumberOfEntities());
	swprintf_s(m_particleCount, L"Particles: %d\n", m_particleManager->GetNumberOfParticles());
#endif

	if (m_playerStatus.IsGameOver())
	{
		game->PushState(std::move(std::make_unique<GameOverState>()));
	}
}

void PlayState::UpdateUserInput(InputManager* inputManager)
{
    GamePad::State state = inputManager->GetGamePadState();
    Keyboard::State kb = inputManager->GetKeyboardState();

    if (m_playerStatus.IsAlive()) 
    {
        TranslationComponent& translation = m_entityManager->GetComponentStore<TranslationComponent>().Get(m_playerStatus.GetCurrentEntityId());
        ProjectileSourceComponent& projectile = m_entityManager->GetComponentStore<ProjectileSourceComponent>().Get(m_playerStatus.GetCurrentEntityId());

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

        float movementSpeed = 6500.0f;
        float drag = 10.0f;

        float accelerationLength = acceleration.LengthSquared();
        if (accelerationLength > 1.0f)
        {
            acceleration *= (1.0f / sqrt(accelerationLength));
        }

        translation.acceleration = (acceleration * movementSpeed) + (translation.velocity * -drag);

		if (translation.acceleration.LengthSquared() < 0.1f)
		{
			translation.acceleration = Vector2::Zero;
		}
    }
    else if (kb.Q || state.IsBPressed())
    {
        SpawnPlayer(false);
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
	if (m_entityManager->GetNumberOfEntities() > 20)
	{
		return;
	}

	if (MathHelper::Random(0, static_cast<int>(m_enemyInverseSpawnChance)) == 0)
	{
		TranslationComponent& playerTranslation = m_entityManager->GetComponentStore<TranslationComponent>().Get(m_playerStatus.GetCurrentEntityId());

		Vector2 spawnPosition = Vector2::Zero;
		int positionChecks = 0;
		do
		{
			spawnPosition = GenerateRandomPosition();
			positionChecks++;
		} while ((Vector2::DistanceSquared(spawnPosition, playerTranslation.position) < std::pow(100.0f, 2)) || positionChecks < 10);

		auto enemy = m_entityManager->CreateEntity();
		m_entityManager->AddComponent(enemy, TranslationComponent(spawnPosition, Vector2(0, 0), MathHelper::Random(0.0f, 6.2f)));
		m_entityManager->AddComponent(enemy, RenderComponent(*m_spriteBatch.get(), m_assetManager->GetTexture(SeekerAEnemyAsset), m_spriteFont.get(), 5, 5));
		m_entityManager->AddComponent(enemy, EnemyComponent(1.0f, 20));
		m_entityManager->AddComponent(enemy, FollowPlayerComponent(&m_playerStatus, 4000.0f, 10.0f));
		m_entityManager->AddComponent(enemy, AvoidanceComponent());
		m_entityManager->AddComponent(enemy, SeparationComponent());
		m_entityManager->AddComponent(enemy, ColliderComponent(15.0f, 35.0f));
		m_entityManager->AddComponent(enemy, DestructableComponent(m_particleManager.get(), m_assetManager->GetTexture(ParticleAsset)));
		m_entityManager->AddComponent(enemy, HealthComponent(10.0f));
		m_entityManager->RegisterEntity(enemy);
	}

	if (MathHelper::Random(0, static_cast<int>(m_enemyInverseSpawnChance)) == 0)
	{
		auto enemy = m_entityManager->CreateEntity();
		m_entityManager->AddComponent(enemy, TranslationComponent(GenerateRandomPosition(), Vector2(0, 0), MathHelper::Random(0.0f, 6.2f)));
		m_entityManager->AddComponent(enemy, RenderComponent(*m_spriteBatch.get(), m_assetManager->GetTexture(WanderEnemyAsset), m_spriteFont.get()));
		m_entityManager->AddComponent(enemy, EnemyComponent(1.2f, 10));
		m_entityManager->AddComponent(enemy, WanderComponent(&m_playerStatus, 2500.0f, 10.0f));
		m_entityManager->AddComponent(enemy, SeparationComponent());
		m_entityManager->AddComponent(enemy, ColliderComponent(15.0f, 35.0f));
		m_entityManager->AddComponent(enemy, DestructableComponent(m_particleManager.get(), m_assetManager->GetTexture(ParticleAsset)));
		m_entityManager->AddComponent(enemy, HealthComponent(20.0f));
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

void PlayState::Resume(std::string previousState)
{
	if (previousState == "GameOverState")
	{
		RestartGame();
	}
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

	RECT outputSize = deviceResources.GetOutputSize();
		
	Matrix viewPortTransform = GetViewportTransform(outputSize);
	m_backgroundBuffer->MatrixTransform = Matrix::Identity * viewPortTransform;
	m_backgroundBuffer->ScrollTransform = m_camera->GetScrollMatrix(
		Vector2(static_cast<float>(m_fixedBackground->GetWidth()), 
			static_cast<float>(m_fixedBackground->GetHeight()))) * viewPortTransform;
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DX::ThrowIfFailed(
		context->Map(m_backgroundD3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
	);
	*static_cast<BackgroundBuffer*>(mappedResource.pData) = *m_backgroundBuffer;
	context->Unmap(m_backgroundD3dBuffer.Get(), 0);

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, m_states->LinearWrap(), nullptr, nullptr, [=]
	{
		context->VSSetShader(m_backgroundVs.Get(), nullptr, 0);
		context->PSSetShader(m_backgroundPs.Get(), nullptr, 0);
		context->VSSetConstantBuffers(0, 1, m_backgroundD3dBuffer.GetAddressOf());
	});
	m_spriteBatch->Draw(m_fixedBackground->GetSrv(), outputSize, &outputSize, Colors::White, 0.0f, Vector2::Zero, SpriteEffects::SpriteEffects_None, 0.0f);
	m_spriteBatch->End();

	for (auto& l : m_backgroundLayers)
	{
		l->Draw(*m_spriteBatch.get());
	}

	Matrix cameraViewMatrix = m_camera->GetViewMatrix();

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, cameraViewMatrix);
	m_entityManager->RenderEntities();
	m_spriteBatch->End();

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Texture, m_states->Additive(), nullptr, nullptr, nullptr, nullptr, cameraViewMatrix);
	m_particleManager->Draw(*m_spriteBatch.get());
	m_spriteBatch->End();

	// TODO: Refactor drawing helpers so we can call draw rect!
	RegionComponent& region = m_entityManager->GetComponentStore<RegionComponent>().Get(m_regionEntity);
	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, cameraViewMatrix);
	Rectangle r = Rectangle(region.min.x, region.min.y, region.max.x, region.max.y);
	m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x, r.y, r.width, 8), DirectX::Colors::White);
	m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x, r.y, 8, r.height), DirectX::Colors::White);
	m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x + r.width - 8, r.y, 8, r.height), DirectX::Colors::White);
	m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x, r.y + r.height - 8, r.width, 8), DirectX::Colors::White);
	m_spriteBatch->End();

	// GUI
	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), m_scoreDisplay, XMFLOAT2(10, 10), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
	m_spriteFont->DrawString(m_spriteBatch.get(), m_multiplierDisplay, XMFLOAT2(10, 30), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
	m_spriteFont->DrawString(m_spriteBatch.get(), m_healthDisplay, XMFLOAT2(10, 50), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
	m_spriteFont->DrawString(m_spriteBatch.get(), m_livesDisplay, XMFLOAT2(10, 70), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
	m_spriteBatch->End();

#if _DEBUG
	if (ShowDebugInformation)
	{
		m_spriteBatch->Begin();
		m_spriteFont->DrawString(m_spriteBatch.get(), m_framesPerSecond, XMFLOAT2(10, 80), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
		m_spriteFont->DrawString(m_spriteBatch.get(), m_entityCount, XMFLOAT2(10, 100), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
		m_spriteFont->DrawString(m_spriteBatch.get(), m_particleCount, XMFLOAT2(10, 120), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
		m_spriteBatch->End();

		m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, cameraViewMatrix);
		std::vector<Rectangle> quadTreeBounds = m_entityManager->GetQuadTree()->GetAllBounds(m_entityManager->GetQuadTree());
		for (size_t i = 0; i < quadTreeBounds.size(); ++i)
		{
			Rectangle r = quadTreeBounds[i];

			m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x, r.y, r.width, 1), DirectX::Colors::White);
			m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x, r.y, 1, r.height), DirectX::Colors::White);
			m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x + r.width - 1, r.y, 1, r.height), DirectX::Colors::White);
			m_spriteBatch->Draw(m_assetManager->GetTexture(DebugAsset)->GetSrv(), Rectangle(r.x, r.y + r.height - 1, r.width, 1), DirectX::Colors::White);
		}
		m_spriteBatch->End();
	}
#endif
};

Matrix PlayState::GetViewportTransform(RECT outputSize)
{
	LONG width = outputSize.right - outputSize.left;
	LONG height = outputSize.bottom - outputSize.top;

	float xScale = (width > 0) ? 2.0f / width : 0.0f;
	float yScale = (height > 0) ? 2.0f / height : 0.0f;

	return XMMATRIX(xScale, 0, 0, 0, 0, -yScale, 0, 0, 0, 0, 1, 0, -1, 1, 0, 1);
}