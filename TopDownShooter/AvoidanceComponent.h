#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct AvoidanceComponent : public Component
{
	static const ComponentType Type = 9;

    DirectX::SimpleMath::Vector2 ahead;
    DirectX::SimpleMath::Vector2 ahead2;
    bool avoiding;

	AvoidanceComponent() :
        ahead(DirectX::SimpleMath::Vector2::Zero),
        ahead2(DirectX::SimpleMath::Vector2::Zero),
        avoiding(false) { }
};
