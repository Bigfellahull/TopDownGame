#include "stdafx.h"

#include "DestructableSystem.h"
#include "DestructableComponent.h"
#include "RenderComponent.h"
#include "TranslationComponent.h"
#include "ColliderComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemDestructable::SystemDestructable(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(DestructableComponent::Type);
	requiredComponents.insert(RenderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(ColliderComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemDestructable::UpdateEntity(float dt, Entity entity)
{
	DestructableComponent& destructable = m_manager.GetComponentStore<DestructableComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);

	if (collider.hasCollided || destructable.destroy)
	{
		m_manager.QueueEntityForDrop(entity);

		float hue1 = MathHelper::Random(0.0f, 6.0f);
		float hue2 = std::fmod(hue1 + MathHelper::Random(0.0f, 2.0f), 6.0f);

		Vector4 colour1 = ColourUtility::HsvToColour(hue1, 0.8f, 1.0f);
		Vector4 colour2 = ColourUtility::HsvToColour(hue2, 0.8f, 1.0f);

		Vector4 colour = Vector4::Lerp(colour1, colour2, MathHelper::Random(0.0f, 1.0f));

		for (int i = 0; i < 200; i++)
		{
			float speed = destructable.particleSpeed * (1.0f - 1 / MathHelper::Random(1.0f, 10.0f));
			float theta = MathHelper::Random(0.0f, 1.0f) * 2.0f * 3.142f;

			Vector2 velocity = Vector2(speed * static_cast<float>(std::cosf(theta)), speed * static_cast<float>(std::sinf(theta)));

			destructable.particleManger->CreateParticle(
				destructable.particleTexture,
				translation.position,
				velocity,
				colour,
				200.0f,
				Vector2(0.7f, 0.7f));
		}
	}
}
