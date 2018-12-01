#include "stdafx.h"

#include "BackgroundLayer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

BackgroundLayer::BackgroundLayer(Camera* camera, Vector2 parallax) :
	m_camera(camera),
	m_sprites(),
	m_parallax(parallax)	
{

}

void BackgroundLayer::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, m_camera->GetViewMatrix(m_parallax));

	for (auto& e : m_sprites)
	{
		spriteBatch.Draw(e.texture->GetSrv(), e.position, Colors::White);
	}

	spriteBatch.End();
}

void BackgroundLayer::AddBackgroundSprite(BackgroundSprite sprite)
{
	m_sprites.push_back(sprite);
}

Vector2 BackgroundLayer::WorldToScreen(Vector2 worldPosition)
{
	return Vector2::Transform(worldPosition, m_camera->GetViewMatrix(m_parallax));
}

Vector2 BackgroundLayer::ScreenToWorld(Vector2 screenPosition)
{
	return Vector2::Transform(screenPosition, m_camera->GetViewMatrix(m_parallax).Invert());
}