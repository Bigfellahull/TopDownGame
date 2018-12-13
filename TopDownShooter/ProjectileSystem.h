#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemProjectile : public System
{
public:
    SystemProjectile(EntityManager& manager);

    virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};