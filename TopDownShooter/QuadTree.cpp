#include "stdafx.h"

#include "QuadTree.h"

using namespace DirectX::SimpleMath;

QuadTree::QuadTree(int level, Rectangle bounds) :
	m_level(level),
	m_bounds(bounds),
	m_entities(),
	m_nodes()
{
}

void QuadTree::Clear()
{
	m_entities.clear();

	for (size_t i = 0; i < m_nodes.size(); ++i)
	{
		m_nodes[i]->Clear();
	}

	m_nodes.clear();
}

void QuadTree::Split()
{
	int subWidth = m_bounds.width / 2;
	int subHeight = m_bounds.height / 2;
	int x = m_bounds.x;
	int y = m_bounds.y;

	m_nodes.push_back(std::move(std::make_unique<QuadTree>(m_level + 1, Rectangle(x + subWidth, y, subWidth, subHeight))));
	m_nodes.push_back(std::move(std::make_unique<QuadTree>(m_level + 1, Rectangle(x, y, subWidth, subHeight))));
	m_nodes.push_back(std::move(std::make_unique<QuadTree>(m_level + 1, Rectangle(x, y + subHeight, subWidth, subHeight))));
	m_nodes.push_back(std::move(std::make_unique<QuadTree>(m_level + 1, Rectangle(x + subWidth, y + subHeight, subWidth, subHeight))));
}

int QuadTree::GetIndex(DirectX::SimpleMath::Vector2 position)
{
	int index = -1;
	long verticalMidpoint = m_bounds.x + (m_bounds.width / 2);
	long horizontalMidpoint = m_bounds.y + (m_bounds.height / 2);

	bool topQuadrant = (position.y < horizontalMidpoint);
	bool bottomQuadrant = position.y > horizontalMidpoint;

	if (position.x < verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = 1;
		}
		else if (bottomQuadrant)
		{
			index = 2;
		}
	}
	else if (position.x > verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = 0;
		}
		else if (bottomQuadrant)
		{
			index = 3;
		}
	}

	return index;
}

void QuadTree::Insert(QuadTreeEntry entry)
{
	if (m_nodes.size() > 0)
	{
		int index = GetIndex(entry.position);

		if (index != -1)
		{
			m_nodes[index]->Insert(entry);
			return;
		}
	}

	m_entities.push_back(entry);

	if (m_entities.size() > m_maxObjects && m_level < m_maxLevels)
	{
		if (m_nodes.size() < 4)
		{
			Split();
		}

		size_t i = 0;
		while (i < m_entities.size())
		{
			QuadTreeEntry toRedistribute = m_entities[i];
			int index = GetIndex(toRedistribute.position);
			if (index != -1)
			{
				m_nodes[index]->Insert(toRedistribute);
				m_entities.erase(m_entities.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}
}

std::vector<Entity> QuadTree::Retrieve(Vector2 position)
{
	std::vector<Entity> ret;
	int index = GetIndex(position);
	if (index != -1 && m_nodes.size() > 0)
	{
		std::vector<Entity> temp = m_nodes[index]->Retrieve(position);
		ret.insert(ret.end(), temp.begin(), temp.end());
	}

	for (size_t i = 0; i < m_entities.size(); ++i)
	{
		ret.push_back(m_entities[i].entity);
	}

	return ret;
}

std::vector<Rectangle> QuadTree::GetAllBounds(QuadTree* node)
{
	std::vector<Rectangle> ret;
	ret.push_back(node->m_bounds);

	if (node->m_nodes.size() <= 0)
	{
		return ret;
	}

	for (size_t i = 0; i < node->m_nodes.size(); ++i)
	{
		std::vector<Rectangle> temp = GetAllBounds(node->m_nodes[i].get());
		ret.insert(ret.end(), temp.begin(), temp.end());
	}

	return ret;
}