#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct TranslationComponent : public Component 
{
	static const ComponentType Type;

	DirectX::SimpleMath::Vector2 acceleration;
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 velocity;
	float orientation;

	TranslationComponent(DirectX::SimpleMath::Vector2 aPosition, DirectX::SimpleMath::Vector2 aVelocity, float aOrientation) :
		acceleration(DirectX::SimpleMath::Vector2(0, 0)), 
		position(aPosition), 
		velocity(aVelocity),
		orientation(aOrientation) { }
};

const ComponentType TranslationComponent::Type = 1;
