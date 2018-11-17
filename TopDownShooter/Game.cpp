#include "stdafx.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()
{
	m_deviceResources = std::make_unique<DX::DeviceResources>(
		DXGI_FORMAT_B8G8R8A8_UNORM, 
		DXGI_FORMAT_D24_UNORM_S8_UINT, 
		2,
		D3D_FEATURE_LEVEL_10_0);

	m_deviceResources->RegisterDeviceNotify(this);
}

void Game::Initialise(HWND window, int width, int height)
{
	m_gamePad = std::make_unique<GamePad>();
	m_keyboard = std::make_unique<Keyboard>();

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);

	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}

void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

void Game::Update(DX::StepTimer const& timer)
{
	auto pad = m_gamePad->GetState(0);
	if (pad.IsConnected())
	{
		if (pad.IsViewPressed())
		{
			ExitGame();
		}
	}

	auto kb = m_keyboard->GetState();
	if (kb.Escape)
	{
		ExitGame();
	}
}

void Game::Render()
{
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	auto context = m_deviceResources->GetD3DDeviceContext();

	m_sprites->Begin();

#if _DEBUG
	wchar_t fpsBuffer[100] = {};
	swprintf_s(fpsBuffer, L"FPS %d\n", m_timer.GetFramesPerSecond());
	m_font->DrawString(m_sprites.get(), fpsBuffer, XMFLOAT2(10, 10), Colors::White, 0.0f, XMFLOAT2(0, 0), 0.7f);
#endif

	m_sprites->End();

	m_deviceResources->Present();
}

void Game::Clear()
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
	{
		return;
	}

	CreateWindowSizeDependentResources();
}

void Game::GetDefaultSize(int& width, int& height) const
{
	width = 1280;
	height = 720;
}

void Game::CreateDeviceDependentResources()
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto device = m_deviceResources->GetD3DDevice();

	m_states = std::make_unique<CommonStates>(device);
	m_sprites = std::make_unique<SpriteBatch>(context);
	m_font = std::make_unique<SpriteFont>(device, L"Fonts\\SegoeUI_18.spritefont");
}

void Game::CreateWindowSizeDependentResources()
{
	auto viewport = m_deviceResources->GetScreenViewport();
	m_sprites->SetViewport(viewport);
}

void Game::OnDeviceLost()
{
	m_states.reset();
	m_sprites.reset();
	m_font.reset();
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
