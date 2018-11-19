#include "stdafx.h"

#include "EntityManager.h"

void EntityManager::Update(DX::StepTimer const& timer, Game* game)
{
    auto size = m_entities.size();

    for (size_t i = 0; i < size; ++i) 
    {
        m_entities[i]->Update(timer, game);
    }

	auto toRemove = std::remove_if(
		std::begin(m_entities), 
		std::end(m_entities), 
		[](const auto& e)
	{
		return !e->IsAlive();
	});

	m_entities.erase(toRemove, std::end(m_entities));
}

void EntityManager::Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex)
{
	for (auto& entity : m_entities)
	{
		entity->Draw(spriteBatch, tex);
	}
}