#include "stdafx.h"

#include "GraphicsHelper.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void GraphicsHelper::DrawLine(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::XMVECTORF32 colour, int lineWidth)
{
	float length = Vector2::Distance(p1, p2);
	float angle = static_cast<float>(std::atan2(p2.y - p1.y, p2.x - p1.x));

	spriteBatch.Draw(texture,
		p1,
		nullptr,
		colour,
		angle,
		Vector2::Zero,
		Vector2(length, static_cast<float>(lineWidth)),
		DirectX::SpriteEffects_None,
		0.0f);
}

void GraphicsHelper::DrawPolygon(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, std::vector<DirectX::SimpleMath::Vector2> verts, int count, DirectX::XMVECTORF32 colour, int lineWidth)
{
	if (count > 0)
	{
		for (int i = 0; i < count - i; i++)
		{
			DrawLine(spriteBatch, texture, verts[i], verts[i + 1], colour, lineWidth);
		}

		DrawLine(spriteBatch, texture, verts[count - 1], verts[0], colour, lineWidth);
	}
}

void GraphicsHelper::DrawCircle(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 centre, float radius, DirectX::XMVECTORF32 colour, int lineWidth, int segments)
{
	std::vector<Vector2> verts = std::vector<Vector2>(segments);

	float increment = 3.142f * 4.0f / segments;
	float theta = 0.0f;

	for (int i = 0; i < segments; i++)
	{
		verts[i] = centre + radius * Vector2(static_cast<float>(std::cosf(theta)), static_cast<float>(std::sinf(theta)));
		theta += increment;
	}

	DrawPolygon(spriteBatch, texture, verts, segments, colour, lineWidth);
}

void GraphicsHelper::DrawRectangle(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Rectangle rectangle, DirectX::XMVECTORF32 colour)
{
	spriteBatch.Draw(texture, rectangle, colour);
}

void GraphicsHelper::DrawRectangleOutline(DirectX::SpriteBatch& spriteBatch, ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Rectangle rectangle, int lineWidth, DirectX::XMVECTORF32 colour)
{
	spriteBatch.Draw(texture, Rectangle(rectangle.x, rectangle.y, rectangle.width, lineWidth), colour);
	spriteBatch.Draw(texture, Rectangle(rectangle.x, rectangle.y, lineWidth, rectangle.height), colour);
	spriteBatch.Draw(texture, Rectangle(rectangle.x + rectangle.width - lineWidth, rectangle.y, lineWidth, rectangle.height), colour);
	spriteBatch.Draw(texture, Rectangle(rectangle.x, rectangle.y + rectangle.height - lineWidth, rectangle.width, lineWidth), colour);
}