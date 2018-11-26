#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemFollowPlayer : public System
{
public:
    SystemFollowPlayer(EntityManager& manager);

    virtual void UpdateEntity(float dt, Entity entity);
};