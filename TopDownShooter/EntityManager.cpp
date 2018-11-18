#include "stdafx.h"

#include "EntityManager.h"

void EntityManager::Add(Entity* entity)
{
	m_addedEntities.push_back(entity);
}

void EntityManager::Update(DX::StepTimer const& timer, Game* game)
{
	for (auto entity : m_entities)
	{
		entity->Update(timer, game);
	}

	for (auto entity : m_addedEntities)
	{
		m_entities.push_back(entity);
	}

	m_addedEntities.clear();
}

void EntityManager::Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex)
{
	for (auto entity : m_entities)
	{
		entity->Draw(spriteBatch, tex);
	}
}