#include "stdafx.h"

#include "Game.h"
#include "IGameState.h"

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
	m_inputManager = std::make_unique<InputManager>();

	m_deviceResources->SetWindow(window, width, height);
	m_deviceResources->CreateDeviceResources();
	m_deviceResources->CreateWindowSizeDependentResources();
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
	m_inputManager->Update();

	m_states.back()->Update(timer, this);

	m_states.back()->HandleStateChange(this);
}

void Game::Render()
{
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	ID3D11DeviceContext1* context = m_deviceResources->GetD3DDeviceContext();
	ID3D11RenderTargetView* renderTarget = m_deviceResources->GetRenderTargetView();
	ID3D11DepthStencilView* depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	D3D11_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_states.back()->Render(*m_deviceResources.get());

	m_deviceResources->Present();
}

void Game::ChangeState(std::unique_ptr<IGameState> state)
{
	if (!m_states.empty())
	{
		m_states.back()->CleanUp();
		m_states.pop_back();
	}

	m_states.push_back(std::move(state));
	m_states.back()->Initialise(*m_deviceResources.get());
}

void Game::PushState(std::unique_ptr<IGameState> state)
{
	if (!m_states.empty())
	{
		m_states.back()->Pause();
	}

	m_states.push_back(std::move(state));
	m_states.back()->Initialise(*m_deviceResources.get());
}

void Game::PopState()
{
	if (!m_states.empty())
	{
		m_states.back()->CleanUp();
		m_states.pop_back();
	}

	if (!m_states.empty()) 
	{
		m_states.back()->Resume();
	}
}

void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
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

	m_states.back()->WindowSizeChanged(m_deviceResources->GetScreenViewport());
}

void Game::Quit()
{
	ExitGame();
}

void Game::GetDefaultSize(int& width, int& height) const
{
	width = 1280;
	height = 720;
}

void Game::OnDeviceLost()
{
	m_states.back()->CleanUp();
}

void Game::OnDeviceRestored()
{
	m_states.back()->Initialise(*m_deviceResources.get());
}
