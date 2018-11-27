#include "stdafx.h"

#include "AvoidanceSystem.h"
#include "AvoidanceComponent.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "PlayerComponent.h"

#include "RenderComponent.h"

using namespace DirectX::SimpleMath;

SystemAvoidance::SystemAvoidance(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(AvoidanceComponent::Type);
	requiredComponents.insert(ColliderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemAvoidance::UpdateEntity(float dt, Entity entity)
{
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

	Vector2 normalisedVelocity = translation.velocity;
	normalisedVelocity.Normalize();
	Vector2 ahead = translation.position + normalisedVelocity * 100.0f;
	Vector2 ahead2 = ahead * 0.5f;

	TranslationComponent* mostThreateningObstancle = nullptr;

	const std::unordered_map<Entity, ColliderComponent>& otherColliders = m_manager.GetComponentStore<ColliderComponent>().GetComponents();
	for (auto e : otherColliders)
	{
		if (e.first == entity || m_manager.GetComponentStore<PlayerComponent>().Has(e.first))
		{
			continue;
		}

		TranslationComponent& otherTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e.first);
		if ((Vector2::DistanceSquared(otherTranslation.position, ahead) <= e.second.radius * e.second.radius) ||
			(Vector2::DistanceSquared(otherTranslation.position, ahead2) <= e.second.radius * e.second.radius))
		{
			if (!mostThreateningObstancle || 
				(Vector2::DistanceSquared(translation.position, otherTranslation.position) < Vector2::DistanceSquared(translation.position, mostThreateningObstancle->position)))
			{
				mostThreateningObstancle = &otherTranslation;
			}
		}
	}


	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);



	Vector2 edge = ahead - translation.position;
	// calculate angle to rotate line
	float angle =
		(float)std::atan2(edge.y, edge.x);

	render.spriteBatch.Begin(DirectX::SpriteSortMode_Deferred);

	render.spriteBatch.Draw(render.texture->GetSrv(),
		RECT{// rectangle defines shape of line and position of start of line
		(int)translation.position.x,
			(int)translation.position.y,
			(int)edge.Length(), //sb will strech the texture to fill this rectangle
			1},
		0,
		DirectX::Colors::Red, //colour of line
		angle,     //angle of line (calulated above)
		Vector2(0, 0), // point in line about which to rotate
		DirectX::SpriteEffects_None,
		0);

	render.spriteBatch.End();

	if (mostThreateningObstancle)
	{
		Vector2 avoidanceForce = ahead - mostThreateningObstancle->position;
		avoidanceForce.Normalize();

		translation.acceleration = (avoidanceForce * 10000.0f) + (translation.velocity);
	}
}