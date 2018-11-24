#pragma once

#include "IGameState.h"
#include "Manager.h"

class PlayState : public IGameState
{
public:
	PlayState() :
		m_framesPerSecond()
	{ }

	void Initialise(DX::DeviceResources const& deviceResources);
	void CleanUp();

	void Pause();
	void Resume();

	void WindowSizeChanged(D3D11_VIEWPORT viewPort);

	void Update(DX::StepTimer const& timer, Game* game);
	void Render(DX::DeviceResources const& deviceResources);
private:
    std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_testTexture;
	std::unique_ptr<Manager> m_manager;

	wchar_t m_framesPerSecond[100];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};