#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemCollider : public System
{
public:
	SystemCollider(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};