#pragma once

#include "Component.h"
#include "SpriteBatch.h"

struct RenderComponent : public Component
{
	static const ComponentType Type;

	DirectX::SpriteBatch& spriteBatch;
	Texture2d* texture;

	RenderComponent(DirectX::SpriteBatch& aSpriteBatch, Texture2d* aTexture) :
		spriteBatch(aSpriteBatch),
		texture(aTexture) { }
};

const ComponentType RenderComponent::Type = 2;