#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

class Game : public DX::IDeviceNotify
{
public:
	Game();

	void Initialise(HWND window, int width, int height);
	void Tick();

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	void GetDefaultSize(int& width, int& height) const;
private:
	void Update(DX::StepTimer const& timer);
	void Render();
	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	std::unique_ptr<DX::DeviceResources> m_deviceResources;

	DX::StepTimer m_timer;

	std::unique_ptr<DirectX::GamePad> m_gamePad;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;
	std::unique_ptr<DirectX::SpriteFont> m_font;
};
