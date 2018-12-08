#pragma once

#include "Camera.h"
#include "SimpleMath.h"
#include "Texture2d.h"
#include "SpriteBatch.h"
#include <vector>

struct BackgroundSprite
{
	DirectX::SimpleMath::Vector2 position;
	Texture2d* texture;
	float scale;

	BackgroundSprite(DirectX::SimpleMath::Vector2 aPosition, Texture2d* aTexture, float aScale = 1.0f) :
		position(aPosition),
		texture(aTexture),
		scale(aScale) { }
};

class BackgroundLayer
{
public:
	BackgroundLayer(Camera* camera, DirectX::SimpleMath::Vector2 parallax);
	void Draw(DirectX::SpriteBatch& spriteBatch);
	DirectX::SimpleMath::Vector2 WorldToScreen(DirectX::SimpleMath::Vector2 worldPosition);
	DirectX::SimpleMath::Vector2 ScreenToWorld(DirectX::SimpleMath::Vector2 screenPosition);
	
	void AddBackgroundSprite(BackgroundSprite sprite);
private:
	Camera* m_camera;
	std::vector<BackgroundSprite> m_sprites;
	DirectX::SimpleMath::Vector2 m_parallax;
};