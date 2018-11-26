#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemProjectileSource : public System
{
public:
    SystemProjectileSource(EntityManager& manager);

    virtual void UpdateEntity(float dt, Entity entity);
};