#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemCollisionHandler : public System
{
public:
	SystemCollisionHandler(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
private:
	void DestroyEntity(Entity entity);
	void DestoryAllDestructableComponents();
};