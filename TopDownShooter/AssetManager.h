#pragma once

#include <map>
#include "Texture2d.h"
#include <memory>

class AssetManager
{
public:
	AssetManager(ID3D11Device1* device) :
		m_device(device) { }

	Texture2d* GetTexture(const wchar_t* filename);

	void DisposeTexture(const wchar_t* filename);

private:
	ID3D11Device1* m_device;
	std::map<const wchar_t*, std::unique_ptr<Texture2d>> m_textures;
};