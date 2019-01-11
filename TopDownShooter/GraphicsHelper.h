#pragma once

#include "SpriteBatch.h"
#include "SimpleMath.h"
#include <vector>

class GraphicsHelper
{
public:
	static void DrawLine(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::XMVECTORF32 colour, int lineWidth);
	static void DrawPolygon(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, std::vector<DirectX::SimpleMath::Vector2> verts, int count, DirectX::XMVECTORF32 colour, int lineWidth);

	static void DrawRectangle(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Rectangle rectangle, DirectX::XMVECTORF32 colour);
	static void DrawRectangleOutline(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Rectangle rectangle, int lineWidth, DirectX::XMVECTORF32 colour);
	static void DrawCircle(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 centre, float radius, DirectX::XMVECTORF32 colour, int lineWidth, int segments = 32);
};