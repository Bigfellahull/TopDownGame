#pragma once

#include "Component.h"
#include "SpriteBatch.h"

struct RenderComponent : public Component
{
	static const ComponentType Type;

	DirectX::SpriteBatch& spriteBatch;
	ID3D11Resource* resource;
	ID3D11ShaderResourceView* texture;

	RenderComponent(DirectX::SpriteBatch& aSpriteBatch, ID3D11ShaderResourceView* aTexture, ID3D11Resource* aResource) :
		spriteBatch(aSpriteBatch),
		resource(aResource),
		texture(aTexture) { }
};

const ComponentType RenderComponent::Type = 2;