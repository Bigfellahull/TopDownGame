#pragma once

#include "Component.h"
#include "PlayerStatus.h"

struct PlayerComponent : public Component
{
    static const ComponentType Type = 8;

    PlayerStatus* status;

    PlayerComponent(PlayerStatus* aStatus) :
        status(aStatus) { }
};
