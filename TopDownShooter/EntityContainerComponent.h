#pragma once

#include "Component.h"
#include "Entity.h"
#include "SimpleMath.h"

struct EntityContainerComponent : public Component
{
	static const ComponentType Type = 18;

	Entity containingEntity;
	int boundsWidth;
	int boundsHeight;
	DirectX::SimpleMath::Rectangle boundingRect;
	bool init;
	DirectX::SimpleMath::Vector2 velocity;

	EntityContainerComponent(Entity aContainingEntity, int aBoundsWidth, int aBoundsHeight) :
		containingEntity(aContainingEntity),
		boundsWidth(aBoundsWidth),
		boundsHeight(aBoundsHeight),
		boundingRect(DirectX::SimpleMath::Rectangle()),
		init(false),
		velocity(DirectX::SimpleMath::Vector2::Zero) { }
};
