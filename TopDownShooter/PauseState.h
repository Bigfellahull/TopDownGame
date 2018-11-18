#pragma once

#include "GameState.h"

class PauseState : public GameState
{
public:
	PauseState() { }

	void Initialise(DX::DeviceResources const& deviceResources);
	void CleanUp();

	void Pause();
	void Resume();

	void WindowSizeChanged(D3D11_VIEWPORT viewPort);

	void Update(DX::StepTimer const& timer, Game* game);
	void Render(DX::DeviceResources const& deviceResources);
};