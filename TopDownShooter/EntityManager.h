#pragma once

#include "Entity.h"
#include <vector>
#include "SpriteBatch.h"

class EntityManager
{
public:
	void Add(std::unique_ptr<Entity> entity);
	void Update(DX::StepTimer const& timer, Game* game);
	void Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex);

private:
	std::vector<std::unique_ptr<Entity>> m_entities;
};