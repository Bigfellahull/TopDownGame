#pragma once

#include "Component.h"

struct PositionComponent : public Component 
{
	static const ComponentType Type;

	float x;
	float y; 

	PositionComponent(float aX, float aY) : x(aX), y(aY) { }
};

const ComponentType PositionComponent::Type = 1;
