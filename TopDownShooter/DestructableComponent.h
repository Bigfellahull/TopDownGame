#pragma once

#include "Component.h"
#include "Camera.h"
#include "Texture2d.h"

struct DestructableComponent : public Component
{
	static const ComponentType Type = 14;

	Texture2d* particleTexture;
	float particleSpeed;
	int numberOfParticles;
	bool collidableParticles;
	bool destroy;
	bool randomColour;
	DirectX::SimpleMath::Vector4 colour;

	// TODO: Refactor this to somewhere more sensible
	Camera* camera;
	
	DestructableComponent(
		Texture2d* aParticleTexture,
		Camera* aCamera,
		float aParticleSpeed = 500.0f, 
		bool collidableParticles = false,
		int aNumberOfParticles = 200, 
		bool aRandomColour = true, 
		DirectX::SimpleMath::Vector4 aColour = DirectX::SimpleMath::Vector4::Zero) :
		particleTexture(aParticleTexture),
		particleSpeed(aParticleSpeed),
		collidableParticles(collidableParticles),
		numberOfParticles(aNumberOfParticles),
		destroy(false),
		randomColour(aRandomColour),
		colour(aColour),
		camera(aCamera) { }
};
