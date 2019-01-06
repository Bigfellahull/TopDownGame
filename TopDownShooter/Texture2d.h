#pragma once

#include <d3d11.h>
#include <wrl/client.h>

class Texture2d
{
public:
	Texture2d() :
		m_srv(),
		m_width(0),
		m_height(0) { };

	void Load(ID3D11Device* device, const wchar_t* filename);

	ID3D11ShaderResourceView* GetSrv() const { return m_srv.Get(); }
	
	const int GetWidth() const { return m_width; }
	const int GetHeight() const { return m_height; }
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	int m_width;
	int m_height;
};