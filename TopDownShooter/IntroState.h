#pragma once

#include "IGameState.h"

class IntroState : public IGameState
{
public:
	IntroState()
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
};