#pragma once

#include "System.h"
#include "EntityManager.h"
#include "Texture2d.h"

class SystemDebugRender : public System
{
public:
    SystemDebugRender(EntityManager& manager, Texture2d* debugTexture);

    virtual void RenderEntity(Entity entity);
private:
    Texture2d* m_debugTexture;
};