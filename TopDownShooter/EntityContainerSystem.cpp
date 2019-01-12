#include "stdafx.h"

#include "EntityContainerSystem.h"
#include "EntityContainerComponent.h"
#include "TranslationComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemEntityContainer::SystemEntityContainer(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(EntityContainerComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemEntityContainer::UpdateEntity(float dt, float totalTime, Entity entity)
{
	EntityContainerComponent& container = m_manager.GetComponentStore<EntityContainerComponent>().Get(entity);

	ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();

	if (!translationComponents.Has(container.containingEntity))
	{
		return;
	}

	TranslationComponent& targetTranslation = translationComponents.Get(container.containingEntity);

	if (!container.init)
	{
		long x = static_cast<long>(targetTranslation.position.x - (container.boundsWidth / 2));
		long y = static_cast<long>(targetTranslation.position.y - (container.boundsHeight / 2));

		container.boundingRect = { x, y,
			static_cast<long>(container.boundsWidth),
			static_cast<long>(container.boundsHeight) };

		container.init = true;
	}

	float shiftX = 0.0f;
	if (targetTranslation.position.x < container.boundingRect.x)
	{
		shiftX = targetTranslation.position.x - container.boundingRect.x;
	}
	else if (targetTranslation.position.x > (container.boundingRect.x + container.boundingRect.width))
	{
		shiftX = targetTranslation.position.x - (container.boundingRect.x + container.boundingRect.width);
	}
	container.boundingRect.x += static_cast<long>(shiftX);

	float shiftY = 0.0f;
	if (targetTranslation.position.y < container.boundingRect.y)
	{
		shiftY = targetTranslation.position.y - container.boundingRect.y;
	}
	else if (targetTranslation.position.y > (container.boundingRect.y + container.boundingRect.height))
	{
		shiftY = targetTranslation.position.y - (container.boundingRect.y + container.boundingRect.height);
	}
	container.boundingRect.y += static_cast<long>(shiftY);

	container.velocity = Vector2(shiftX, shiftY);
}
