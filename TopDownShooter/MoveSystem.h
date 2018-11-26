#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemMove : public System 
{
public:
    SystemMove(EntityManager& manager);

    virtual void UpdateEntity(float dt, Entity entity);
};