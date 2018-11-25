#include "stdafx.h"

#include "AssetManager.h"

Texture2d* AssetManager::GetTexture(const wchar_t* filename)
{
	if (m_textures.end() == m_textures.find(filename))
	{
		m_textures.insert(std::make_pair(filename, std::make_unique<Texture2d>()));
		m_textures[filename]->Load(m_device, filename);
	}

	return m_textures[filename].get();
}

void AssetManager::DisposeTexture(const wchar_t* filename)
{
	if (m_textures.end() != m_textures.find(filename))
	{
		m_textures.erase(filename);
	}
}
