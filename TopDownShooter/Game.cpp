#include "stdafx.h"

#include "Game.h"
#include "GameState.h"

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

	m_currentState->Update(timer, this);
}

void Game::Render()
{
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_currentState->Render(*m_deviceResources.get());

	m_deviceResources->Present();
}

void Game::ChangeCurrentState(std::unique_ptr<GameState> state)
{
	state->Initialise(*m_deviceResources.get());

	if (m_currentState)
	{
		m_currentState->CleanUp();
	}
	
	m_currentState.swap(state);
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

	m_currentState->WindowSizeChanged(m_deviceResources->GetScreenViewport());
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
	m_currentState->CleanUp();
}

void Game::OnDeviceRestored()
{
	m_currentState->Initialise(*m_deviceResources.get());
}
