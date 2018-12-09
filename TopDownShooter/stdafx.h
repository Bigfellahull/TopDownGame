#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0602 // Ensure we are using XAudio 2.8
#include <SDKDDKVer.h>

#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <vector>
#include <stdio.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "MathHelper.h"
#include "Config.h"
#include "ColourUtility.h"
#include "ReadData.h"

namespace DX
{
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : 
			result(hr) 
		{ }

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", 
				static_cast<unsigned int>(result));

			return s_str;
		}

	private:
		HRESULT result;
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}