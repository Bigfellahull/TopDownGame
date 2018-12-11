#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemDestructable : public System
{
public:
	SystemDestructable(EntityManager& manager);

	virtual void UpdateEntity(float dt, Entity entity);
};