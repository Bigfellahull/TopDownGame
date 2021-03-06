#pragma once

#include "IGameState.h"
#include "EntityManager.h"
#include "AssetManager.h"

#include "PlayerStatus.h"
#include "Camera.h"
#include "BackgroundLayer.h"

struct BackgroundBuffer
{
	DirectX::SimpleMath::Matrix MatrixTransform;
	DirectX::SimpleMath::Matrix ScrollTransform;
};

class PlayState : public IGameState
{
public:
	PlayState() :
		m_playerStatus(),
		m_regionEntity(),
		m_fixedBackground(nullptr),
		m_enemyInverseSpawnChance(120.0f),
		m_scoreDisplay(),
		m_multiplierDisplay(),
		m_healthDisplay(),
		m_livesDisplay()
#if _DEBUG
		,m_framesPerSecond(),
		m_entityCount()
#endif
	{
		name = __func__;
	}

	void Initialise(DX::DeviceResources const& deviceResources);
	void CleanUp();

	void Pause();
	void Resume(std::string previousState);

	void WindowSizeChanged(D3D11_VIEWPORT viewPort);

	void Update(DX::StepTimer const& timer, Game* game);
	void HandleStateChange(Game* game);
	void Render(DX::DeviceResources const& deviceResources);
private:
	void RestartGame();
	void UpdateUserInput(InputManager* inputManager);
	void SpawnEnemies(float dt);
	DirectX::SimpleMath::Vector2 GenerateRandomPosition();
    void SpawnPlayer(bool reset);
	DirectX::SimpleMath::Matrix GetViewportTransform(RECT outputSize);
    
    std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	
    PlayerStatus m_playerStatus;
	Entity m_cameraPlayerContainer;
	Entity m_regionEntity;
	
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<AssetManager> m_assetManager;
	std::unique_ptr<EntityManager> m_entityManager;
	
	Texture2d* m_fixedBackground;
	std::vector<std::unique_ptr<BackgroundLayer>> m_backgroundLayers;
	std::unique_ptr<BackgroundBuffer> m_backgroundBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_backgroundD3dBuffer;

	float m_enemyInverseSpawnChance;

	wchar_t m_scoreDisplay[100];
	wchar_t m_multiplierDisplay[100];
	wchar_t m_healthDisplay[100];
	wchar_t m_livesDisplay[100];

#if _DEBUG
	wchar_t m_framesPerSecond[100];
	wchar_t m_entityCount[100];
#endif

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_backgroundPs;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_backgroundVs;
};