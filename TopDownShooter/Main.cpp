#include "stdafx.h"

#include "Game.h"
#include "IntroState.h"

using namespace DirectX;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace
{
	std::unique_ptr<Game> g_game;
}

int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		return 1;
	}

	g_game = std::make_unique<Game>();

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TopDownShooter";
	wcex.hIconSm = nullptr;

	if (!RegisterClassEx(&wcex))
	{
		return 1;
	}

	int w, h;
	g_game->GetDefaultSize(w, h);

	RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hWnd = CreateWindowEx(
		0,
		L"TopDownShooter",
		L"TopDownShooter",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (!hWnd)
	{
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, 
		reinterpret_cast<LONG_PTR>(g_game.get()));

	GetClientRect(hWnd, &rc);

	g_game->Initialise(hWnd, rc.right - rc.left, rc.bottom - rc.top);
	g_game->ChangeState(std::move(std::make_unique<IntroState>()));

	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_game->Tick();
		}
	}

	g_game.reset();

	CoUninitialize();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_inSizeMove = false;
	static bool s_inSuspend = false;
	static bool s_minimised = false;
	static bool s_fullscreen = false;

	Game* game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		break;
	}
	case WM_PAINT:
	{
		if (s_inSizeMove && game)
		{
			game->Tick();
		}
		else
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimised)
			{
				s_minimised = true;
				if (!s_inSuspend && game)
				{
					game->OnSuspending();
				}
				s_inSuspend = true;
			}
		}
		else if (s_minimised)
		{
			s_minimised = false;
			if (s_inSuspend && game)
			{
				game->OnResuming();
			}
			s_inSuspend = false;
		}
		else if (!s_inSizeMove && game)
		{
			game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	}
	case WM_ENTERSIZEMOVE:
	{
		s_inSizeMove = true;
		break;
	}
	case WM_EXITSIZEMOVE:
	{
		s_inSizeMove = false;
		if (game)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;
	}
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
		break;
	}
	case WM_ACTIVATEAPP:
	{
		if (game)
		{
			Keyboard::ProcessMessage(message, wParam, lParam);
			Mouse::ProcessMessage(message, wParam, lParam);
			if (wParam)
			{
				game->OnActivated();
			}
			else
			{
				game->OnDeactivated();
			}
		}
		break;
	}
	case WM_POWERBROADCAST:
	{
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_inSuspend && game)
			{
				game->OnSuspending();
			}
			s_inSuspend = true;
			return TRUE;
		case PBT_APMRESUMESUSPEND:
			if (!s_minimised)
			{
				if (s_inSuspend && game)
				{
					game->OnResuming();
				}
				s_inSuspend = false;
			}
			return TRUE;
		}
		break;
	}
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}

	return 0;
}

void ExitGame()
{
	PostQuitMessage(0);
}