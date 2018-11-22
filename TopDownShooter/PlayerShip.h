#pragma once

#include "Entity.h"
#include "SimpleMath.h"

class PlayerShip : public Entity
{
public:
    PlayerShip(DirectX::XMFLOAT2 position) :
        m_position(position),
        m_orientation(0)

    {
    }

    void Update(DX::StepTimer const& timer, Game* game)
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

        m_velocity = 10.0f * direction;
        m_position += m_velocity;
                
        if (m_velocity.LengthSquared() > 0)
        {
            m_orientation = static_cast<float>(std::atan2(m_velocity.y, m_velocity.x));
        }
    }

    virtual void Draw(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* tex)
    {
        spriteBatch.Draw(tex, m_position, nullptr, DirectX::Colors::White, m_orientation);
    }
private:
    DirectX::SimpleMath::Vector2 m_position;
    DirectX::SimpleMath::Vector2 m_velocity;
    float m_orientation;
};
