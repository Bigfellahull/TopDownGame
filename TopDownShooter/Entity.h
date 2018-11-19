#pragma once

#include "Game.h"
#include "SpriteBatch.h"

class EntityManager;

class Entity
{
	friend class EntityManager;

public:
	Entity() { }

	virtual ~Entity() { }

    virtual void Update(DX::StepTimer const& timer, Game* game) = 0;
	
    virtual void Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex) = 0;

	void Kill() { m_alive = false; }

	bool IsAlive() { return m_alive; }
protected:
	bool m_alive;
};
