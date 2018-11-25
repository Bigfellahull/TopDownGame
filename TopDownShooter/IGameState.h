#pragma once

#include "StepTimer.h"
#include "Game.h"

interface IGameState
{
public:
	virtual void Initialise(DX::DeviceResources const& deviceResources) = 0;
	virtual void CleanUp() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void WindowSizeChanged(D3D11_VIEWPORT viewPort) = 0;

	virtual void Update(DX::StepTimer const& timer, Game* game) = 0;
	virtual void HandleStateChange(Game* game) = 0;
	virtual void Render(DX::DeviceResources const& deviceResources) = 0;	
};