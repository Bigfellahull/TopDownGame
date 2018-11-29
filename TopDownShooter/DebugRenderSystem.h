#pragma once

#include "System.h"
#include "EntityManager.h"
#include "Texture2d.h"

class SystemDebugRender : public System
{
public:
    SystemDebugRender(EntityManager& manager, Texture2d* debugTexture);

    virtual void RenderEntity(Entity entity);
private:
    void DrawLine(DirectX::SpriteBatch& spriteBatch, DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::XMVECTORF32 colour, int lineWidth);
    void DrawPolygon(DirectX::SpriteBatch& spriteBatch, std::vector<DirectX::SimpleMath::Vector2> verts, int count, DirectX::XMVECTORF32 colour, int lineWidth);
    void DrawCircle(DirectX::SpriteBatch& spriteBatch, DirectX::SimpleMath::Vector2 centre, float radius, DirectX::XMVECTORF32 colour, int lineWidth, int segments = 32);
    Texture2d* m_debugTexture;
};