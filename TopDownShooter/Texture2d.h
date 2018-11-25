#pragma once

#include <d3d11.h>
#include <wrl/client.h>

class Texture2d
{
public:
	Texture2d() { };

	void Load(ID3D11Device* device, const wchar_t* filename);

	ID3D11ShaderResourceView* GetSrv() const { return m_srv.Get(); }
	
	const float GetWidth() const { return m_width; }
	const float GetHeight() const { return m_height; }
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	float m_width;
	float m_height;
};