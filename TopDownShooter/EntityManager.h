#pragma once

#include "Entity.h"
#include <vector>
#include "SpriteBatch.h"

class EntityManager
{
public:
	template <typename T, typename... TArgs>
	auto& Add(TArgs&&... mArgs)
	{
		m_entities.emplace_back(
			std::make_unique<T>(std::forward<decltype(mArgs)>(mArgs)...));

		return *(m_entities.back());
	}

	void Update(DX::StepTimer const& timer, Game* game);
	void Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex);

private:
	std::vector<std::unique_ptr<Entity>> m_entities;
};