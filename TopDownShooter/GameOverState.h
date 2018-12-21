#pragma once

#include "IGameState.h"

class GameOverState : public IGameState
{
public:
	GameOverState() 
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
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
};