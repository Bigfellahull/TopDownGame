#include "stdafx.h"

#include "EnemyActivatorSystem.h"
#include "EnemyComponent.h"
#include "RenderComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemEnemyActivator::SystemEnemyActivator(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(EnemyComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemEnemyActivator::UpdateEntity(float dt, float totalTime, Entity entity)
{
	EnemyComponent& enemy = m_manager.GetComponentStore<EnemyComponent>().Get(entity);
	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);

	if (enemy.timeToStart > 0.0f)
	{
		enemy.timeToStart -= dt;

		render.colour = Colors::White * (1.0f - static_cast<float>(enemy.timeToStart) / enemy.initialTimeToStart);
		render.scale = 1.6f - (enemy.timeToStart / enemy.initialTimeToStart);
	}
	else
	{
		if (!enemy.alive)
		{
			render.colour = Colors::White;
			render.scale = 1.0f;

			enemy.alive = true;
		}
	}
}
