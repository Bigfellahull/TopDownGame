#pragma once

#include "Component.h"
#include "SpriteBatch.h"
#include "Texture2d.h"

struct RenderComponent : public Component
{
	static const ComponentType Type = 6;

	DirectX::SpriteBatch& spriteBatch;
	DirectX::SpriteFont* spriteFont;
	Texture2d* texture;
	DirectX::SimpleMath::Vector4 colour;
	float scale;

	RenderComponent(DirectX::SpriteBatch& aSpriteBatch, Texture2d* aTexture, DirectX::SpriteFont* aSpriteFont = nullptr) :
		spriteBatch(aSpriteBatch),
		spriteFont(aSpriteFont),
		texture(aTexture),
		colour(DirectX::Colors::White),
		scale(1.0f) { }
};
