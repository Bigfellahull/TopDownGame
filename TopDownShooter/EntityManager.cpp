#include "stdafx.h"

#include "EntityManager.h"

void EntityManager::Add(std::unique_ptr<Entity> entity)
{
	m_entities.push_back(std::move(entity));
}

void EntityManager::Update(DX::StepTimer const& timer, Game* game)
{
    auto size = m_entities.size();

    for (auto i = 0; i < size; ++i) 
    {
        m_entities[i]->Update(timer, game);
    }
}

void EntityManager::Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex)
{
	for (auto& entity : m_entities)
	{
		entity->Draw(spriteBatch, tex);
	}
}