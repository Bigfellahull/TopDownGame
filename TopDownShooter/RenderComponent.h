#pragma once

#include "Component.h"
#include "SpriteBatch.h"
#include "Texture2d.h"

struct RenderComponent : public Component
{
	static const ComponentType Type = 6;

	DirectX::SpriteBatch& spriteBatch;
	Texture2d* texture;
	DirectX::SimpleMath::Vector4 colour;
	float scale;

	RenderComponent(DirectX::SpriteBatch& aSpriteBatch, Texture2d* aTexture) :
		spriteBatch(aSpriteBatch),
		texture(aTexture),
		colour(DirectX::Colors::White),
		scale(1.0f) { }
};
