#pragma once

#include "IGameState.h"
#include "EntityManager.h"
#include "AssetManager.h"

#include "PlayerStatus.h"

class PlayState : public IGameState
{
public:
	PlayState() :
		m_playerStatus(),
		m_regionEntity() { }

	void Initialise(DX::DeviceResources const& deviceResources);
	void CleanUp();

	void Pause();
	void Resume();

	void WindowSizeChanged(D3D11_VIEWPORT viewPort);

	void Update(DX::StepTimer const& timer, Game* game);
	void HandleStateChange(Game* game);
	void Render(DX::DeviceResources const& deviceResources);
private:
	void UpdateUserInput(InputManager* inputManager);
	void SpawnEnemies(float dt);
	DirectX::SimpleMath::Vector2 GenerateRandomPosition();
    void SpawnPlayer();
    
    std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	
    PlayerStatus m_playerStatus;
	Entity m_regionEntity;
	
	std::unique_ptr<AssetManager> m_assetManager;
	std::unique_ptr<EntityManager> m_entityManager;

	float m_enemyInverseSpawnChance;

	wchar_t m_framesPerSecond[100];
	wchar_t m_entityCount[100];
};