#include "stdafx.h"

#include "Texture2d.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

void Texture2d::Load(ID3D11Device* device, const wchar_t* filename)
{
	ID3D11Resource* resource;

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, filename, &resource, m_srv.ReleaseAndGetAddressOf())
	);

	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC description;
	texture->GetDesc(&description);

	m_width = static_cast<int>(description.Width);
	m_height = static_cast<int>(description.Height);
}