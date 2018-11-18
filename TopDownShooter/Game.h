#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "InputManager.h"

class GameState;

class Game : public DX::IDeviceNotify
{
public:
	Game();

	void Initialise(HWND window, int width, int height);
	void Tick();

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	void ChangeState(std::unique_ptr<GameState> state);
	void PushState(std::unique_ptr<GameState> state);
	void PopState();

	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	void Quit();

	void GetDefaultSize(int& width, int& height) const;

	DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }
	InputManager* GetInputManager() { return m_inputManager.get(); }

private:
	void Update(DX::StepTimer const& timer);
	void Render();
	
	DX::StepTimer m_timer;

	std::unique_ptr<DX::DeviceResources> m_deviceResources;
	std::vector<std::unique_ptr<GameState>> m_states;
	std::unique_ptr<InputManager> m_inputManager;
};
