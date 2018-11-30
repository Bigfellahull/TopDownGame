#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct AvoidanceComponent : public Component
{
	static const ComponentType Type = 9;

    DirectX::SimpleMath::Vector2 ahead;
    bool avoiding;
	std::vector<DirectX::SimpleMath::Vector2> debugProjectVector;
	std::vector<DirectX::SimpleMath::Vector2> debugProjectedVector;

	AvoidanceComponent() :
        ahead(DirectX::SimpleMath::Vector2::Zero),
        avoiding(false),
		debugProjectVector(),
		debugProjectedVector() { }
};
