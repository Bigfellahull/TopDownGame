#include "stdafx.h"

#include "FollowPlayerSystem.h"
#include "TranslationComponent.h"
#include "FollowPlayerComponent.h"
#include "EnemyComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemFollowPlayer::SystemFollowPlayer(EntityManager& manager) :
    System(manager, false)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(FollowPlayerComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(EnemyComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemFollowPlayer::UpdateEntity(float dt, Entity entity)
{
    FollowPlayerComponent& follow = m_manager.GetComponentStore<FollowPlayerComponent>().Get(entity);
	if (!follow.playerStatus->isAlive)
	{
		m_manager.QueueEntityForDrop(entity);
		return;
	}

	EnemyComponent& enemy = m_manager.GetComponentStore<EnemyComponent>().Get(entity);

    ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();
	
    if(enemy.alive)
    {
		TranslationComponent& translation = translationComponents.Get(entity);
        TranslationComponent& playerTranslation = translationComponents.Get(follow.playerStatus->currentEntityId);
        Vector2 delta = playerTranslation.position - translation.position;
               
        if (delta.Length() > 50.0f)
        {
            delta.Normalize();

            float accLength = delta.LengthSquared();
            if (accLength > 1.0f)
            {
                delta *= (1.0f / sqrt(accLength));
            }

            translation.acceleration = (delta * follow.movementSpeed) + (translation.velocity * -follow.drag);
        }
        else
        {
            translation.acceleration = translation.velocity * -follow.drag;
        }
    }
}