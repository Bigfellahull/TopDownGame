#pragma once

#include "IGameState.h"

class PlayState : public IGameState
{
public:
	PlayState() { }

	void Initialise(DX::DeviceResources const& deviceResources);
	void CleanUp();

	void Pause();
	void Resume();

	void WindowSizeChanged(D3D11_VIEWPORT viewPort);

	void Update(DX::StepTimer const& timer, Game* game);
	void Render(DX::DeviceResources const& deviceResources);
private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	wchar_t m_framesPerSecond[100];
};