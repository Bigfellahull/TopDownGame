#pragma once

#include "StepTimer.h"
#include "Game.h"

class GameState
{
public:
	GameState() { }

	virtual void Initialise(DX::DeviceResources const& deviceResources) = 0;
	virtual void CleanUp() = 0;

	virtual void WindowSizeChanged(D3D11_VIEWPORT viewPort) = 0;

	virtual void Update(DX::StepTimer const& timer, Game* game) = 0;
	virtual void Render(DX::DeviceResources const& deviceResources) = 0;
};