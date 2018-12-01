#pragma once

#include "Entity.h"
#include "SimpleMath.h"
#include <vector>
#include <memory>

struct QuadTreeEntry
{
	Entity entity;
	DirectX::SimpleMath::Vector2 position;

	QuadTreeEntry(Entity aEntity, DirectX::SimpleMath::Vector2 aPosition) :
		entity(aEntity),
		position(aPosition) { }
};

class QuadTree
{
public:
	QuadTree(int level, DirectX::SimpleMath::Rectangle bounds);
	void Insert(QuadTreeEntry entry);
	void Clear();
	std::vector<Entity> Retrieve(DirectX::SimpleMath::Vector2 position);

    static std::vector<DirectX::SimpleMath::Rectangle> GetAllBounds(QuadTree* node);
private:
	int GetIndex(DirectX::SimpleMath::Vector2 position);
	void Split();

	const size_t m_maxObjects = 10;
	const int m_maxLevels = 2;

	int m_level;
	std::vector<QuadTreeEntry> m_entities;
	DirectX::SimpleMath::Rectangle m_bounds;

	std::vector<std::unique_ptr<QuadTree>> m_nodes;
};
