#pragma once

#include "Component.h"
#include "Texture2d.h"
#include "SpriteBatch.h"

struct RenderComponent : public Component
{
	static const ComponentType Type = 6;

	DirectX::SpriteBatch& spriteBatch;
	DirectX::SpriteFont* spriteFont;
	
	Texture2d* texture;
	bool paused;
	int frame;
	int frameCount;
	float timePerFrame;
	float totalElapsed;
	DirectX::SimpleMath::Vector2 scale;
	DirectX::SimpleMath::Vector2 origin;
	DirectX::SimpleMath::Vector4 colour;

	RenderComponent(DirectX::SpriteBatch& aSpriteBatch, Texture2d* aTexture, DirectX::SpriteFont* aSpriteFont = nullptr, int frameCount = 1, int framesPerSecond = 1) :
		spriteBatch(aSpriteBatch),
		spriteFont(aSpriteFont),
		texture(aTexture),
		paused(false),
		frame(0),
		frameCount(frameCount),
		timePerFrame(1.0f / static_cast<float>(framesPerSecond)),
		totalElapsed(0.0f),
		scale(1.0f, 1.0f),
		origin(static_cast<float>(aTexture->GetWidth() / frameCount / 2), static_cast<float>(aTexture->GetHeight() / 2)),
		colour(DirectX::Colors::White) { }
};
