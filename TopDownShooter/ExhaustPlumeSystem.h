#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemExhaustPlume : public System
{
public:
	SystemExhaustPlume(EntityManager& manager);

	virtual void UpdateEntity(float dt, Entity entity);
};