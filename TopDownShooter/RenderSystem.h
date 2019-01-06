#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemRender : public System
{
public:
    SystemRender(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
    virtual void RenderEntity(Entity entity);
};