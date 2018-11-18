#pragma once

#include "Entity.h"
#include <vector>
#include "SpriteBatch.h"

class EntityManager
{
public:
	void Add(Entity* entity);
	void Update(DX::StepTimer const& timer, Game* game);
	void Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex);

private:
	std::vector<Entity*> m_entities;
	std::vector<Entity*> m_addedEntities;
};