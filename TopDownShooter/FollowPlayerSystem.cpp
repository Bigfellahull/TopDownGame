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
	
	if (!m_manager.GetComponentStore<EnemyComponent>().Get(entity).alive ||
		!follow.playerStatus->isAlive)
	{
		return;
	}

    ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();
    
	TranslationComponent& translation = translationComponents.Get(entity);
    TranslationComponent& playerTranslation = translationComponents.Get(follow.playerStatus->currentEntityId);

	Vector2 playerFuturePosition = playerTranslation.position + playerTranslation.velocity * 0.3f;

    Vector2 delta = playerFuturePosition - translation.position;

    if (delta.LengthSquared() > std::pow(50.0f, 2))
    {
        delta.Normalize();

        float normalisedLength = delta.LengthSquared();
        if (normalisedLength > 1.0f)
        {
            delta *= (1.0f / sqrt(normalisedLength));
        }

        translation.acceleration = (delta * follow.movementSpeed) + (translation.velocity * -follow.drag);
    }
    else
    {
        translation.acceleration = translation.velocity * -follow.drag;
    }
}