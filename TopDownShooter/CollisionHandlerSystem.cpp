#include "stdafx.h"

#include "CollisionHandlerSystem.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "DestructableComponent.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"

using namespace DirectX::SimpleMath;

SystemCollisionHandler::SystemCollisionHandler(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(ColliderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	
	SetRequiredComponents(std::move(requiredComponents));
}

void SystemCollisionHandler::UpdateEntity(float dt, float totalTime, Entity entity)
{
	ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);
	
	if (!collider.hasCollided)
	{
		return;
	}

	ComponentStore<PlayerComponent>& playerComponents = m_manager.GetComponentStore<PlayerComponent>();
	ComponentStore<EnemyComponent>& enemyComponents = m_manager.GetComponentStore<EnemyComponent>();
	ComponentStore<ProjectileComponent>& projectileComponents = m_manager.GetComponentStore<ProjectileComponent>();
	ComponentStore<HealthComponent>& healthComponents = m_manager.GetComponentStore<HealthComponent>();

	if (playerComponents.Has(entity))
	{
		if (enemyComponents.Has(collider.collidedWith))
		{
			if (healthComponents.Has(entity))
			{
				HealthComponent& health = healthComponents.Get(entity);
				health.hitPoints -= 10.0f;

				if (health.hitPoints <= 0.0f)
				{
					PlayerComponent& player = playerComponents.Get(entity);
					player.status->SetAlive(false);
					player.status->RemoveLife();

					DestoryAllDestructableComponents();
				}
			}
		}		
	}

	if (enemyComponents.Has(entity))
	{
		EnemyComponent& enemy = enemyComponents.Get(entity);

		if (playerComponents.Has(collider.collidedWith))
		{
			DestroyEntity(entity);
		}

		if (projectileComponents.Has(collider.collidedWith))
		{
			ProjectileComponent& projectile = projectileComponents.Get(collider.collidedWith);
			if (healthComponents.Has(entity))
			{
				HealthComponent& health = healthComponents.Get(entity);
				health.hitPoints -= projectile.damage;

				if (health.hitPoints <= 0.0f)
				{		
					if (playerComponents.Has(projectile.firedByEntity))
					{
						PlayerComponent& player = playerComponents.Get(projectile.firedByEntity);
						player.status->AddPoints(enemy.points);
						player.status->IncreaseMultiplier();
					}

					DestroyEntity(entity);
				}
			}
		}
	}

	if (projectileComponents.Has(entity))
	{
		DestroyEntity(entity);
	}
}

void SystemCollisionHandler::DestroyEntity(Entity entity)
{
	ComponentStore<DestructableComponent>& destructableComponents = m_manager.GetComponentStore<DestructableComponent>();

	if (destructableComponents.Has(entity))
	{
		DestructableComponent& destructable = destructableComponents.Get(entity);
		destructable.destroy = true;
	}
}

void SystemCollisionHandler::DestoryAllDestructableComponents()
{
	ComponentStore<DestructableComponent>& destructableComponents = m_manager.GetComponentStore<DestructableComponent>();
	for (auto& x : destructableComponents.GetComponents())
	{
		destructableComponents.Get(x.first).destroy = true;
	}
}