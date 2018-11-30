#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemEnemyActivator : public System
{
public:
	SystemEnemyActivator(EntityManager& manager);

	virtual void UpdateEntity(float dt, Entity entity);
};