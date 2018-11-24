#pragma once

#include "Component.h"

struct TranslationComponent : public Component 
{
	static const ComponentType Type;

	DirectX::SimpleMath::Vector2 acceleration;
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 velocity;
	float orientation;

	TranslationComponent(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 vel, float o) :
		acceleration(DirectX::SimpleMath::Vector2(0, 0)), position(pos), velocity(vel), orientation(o) { }
};

const ComponentType TranslationComponent::Type = 1;
