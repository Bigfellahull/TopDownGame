#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemRender : public System
{
public:
    SystemRender(EntityManager& manager);

    virtual void RenderEntity(Entity entity);
};