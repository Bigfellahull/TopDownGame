#pragma once

#include "SimpleMath.h"
#include "Game.h"
#include "SpriteBatch.h"

class EntityManager;

class Entity
{
	friend class EntityManager;

public:
	Entity(DirectX::XMFLOAT2 position)
		: m_position(position)
	{
	}

	virtual ~Entity() { }

	virtual void Update(DX::StepTimer const& timer, Game* game) 
	{ 
		auto inputManager = game->GetInputManager();
		auto state = inputManager->GetGamePadState();

		auto direction = DirectX::SimpleMath::Vector2(state.thumbSticks.leftX, state.thumbSticks.leftY);
		direction.y *= -1;

		auto kb = inputManager->GetKeyboardState();
		if (kb.Up)
		{
			direction.y = -1.0f;
		}
		if (kb.Right)
		{
			direction.x = 1.0f;
		}
		if (kb.Down)
		{
			direction.y = 1.0f;
		}
		if (kb.Left)
		{
			direction.x = -1.0f;
		}

		if (direction.LengthSquared() > 1)
		{
			direction.Normalize();
		}

		auto t = DirectX::SimpleMath::Vector2(m_position);

		t += 10.0f * direction;

		m_position = t;
	}

	virtual void Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex)
	{
		spriteBatch.Draw(tex, m_position, DirectX::Colors::White);
	}

	void Kill() { m_alive = false; }
private:
	bool m_alive;
	DirectX::XMFLOAT2 m_position;
};
