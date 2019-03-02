#include "stdafx.h"

#include "AttractionSystem.h"
#include "AttractionSourceComponent.h"
#include "TranslationComponent.h"

using namespace DirectX::SimpleMath;

SystemAttraction::SystemAttraction(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(TranslationComponent::Type);
	
	SetRequiredComponents(std::move(requiredComponents));
}

void SystemAttraction::UpdateEntity(float dt, float totalTime, Entity entity)
{
	// We shouldn't attract other sources.
	if (m_manager.GetComponentStore<AttractionSourceComponent>().Has(entity))
	{
		return;
	}

	ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();
	ComponentStore<AttractionSourceComponent>& attractionSourceComponents = m_manager.GetComponentStore<AttractionSourceComponent>();

	TranslationComponent& translation = translationComponents.Get(entity);

	std::vector<Entity> nearTranslationEntities = m_manager.GetQuadTree()->Retrieve(translation.position);

	for (auto e : nearTranslationEntities)
	{		
		if (e == entity || !attractionSourceComponents.Has(e))
		{
			continue;
		}

		TranslationComponent& sourceTranslation = translationComponents.Get(e);

		if (Vector2::DistanceSquared(sourceTranslation.position, translation.position) <= std::pow(400.0f, 2))
		{
			Vector2 deltaPosition = sourceTranslation.position - translation.position;
			float distance = deltaPosition.Length();
			deltaPosition.Normalize();

			Vector2 f = (100000.0f * deltaPosition) / (distance * distance + 10000.0f);

			if (distance < 200.0f)
			{
				f += 10000.0f * Vector2(deltaPosition.y, -deltaPosition.x) / (distance + 100.0f);
			}

			translation.acceleration += f;
		}
	}
}
