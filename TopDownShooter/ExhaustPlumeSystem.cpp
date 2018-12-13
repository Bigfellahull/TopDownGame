#include "stdafx.h"

#include "ExhaustPlumeSystem.h"
#include "ExhaustPlumeComponent.h"
#include "TranslationComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemExhaustPlume::SystemExhaustPlume(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(ExhaustPlumeComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	
	SetRequiredComponents(std::move(requiredComponents));
}

void SystemExhaustPlume::UpdateEntity(float dt, Entity entity)
{
	ExhaustPlumeComponent& exhaustPlume = m_manager.GetComponentStore<ExhaustPlumeComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

	if (translation.velocity.LengthSquared() < 1.0f)
	{
		return;
	}

	float orientation = std::atan2(translation.velocity.y, translation.velocity.x);
	Quaternion rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, orientation);

	Vector2 baseVelocity = translation.velocity * (-3.0f / translation.velocity.Length());
	Vector2 perpendicularVelocity = Vector2(baseVelocity.y, -baseVelocity.x) * (0.9f * static_cast<float>(std::sin(dt * 10.0f)));

	Vector4 colour1 = Vector4(0.78f, 0.15f, 0.04f, 1.0f);
	Vector4 colour2 = Vector4(1.0f, 0.73f, 0.12f, 1.0f);
	Vector2 position = translation.position + Vector2::Transform(Vector2(-22.0f, 0.0f), rotation);

	float alpha = 0.7f;

	Vector2 velocityMid = baseVelocity + MathHelper::NextVector2(0.0f, 1.0f);

	exhaustPlume.particleManger->CreateParticle(
		exhaustPlume.particleTexture,
		position,
		velocityMid,
		Vector4::One * alpha,
		40.0f,
		Vector2(0.4f, 0.7f));

	exhaustPlume.particleManger->CreateParticle(
		exhaustPlume.particleGlowTexture,
		position,
		velocityMid,
		colour2 * alpha,
		40.0f,
		Vector2(0.4f, 0.7f));

	Vector2 velocitySide1 = baseVelocity + perpendicularVelocity + MathHelper::NextVector2(0.0f, 0.3f);
	Vector2 velocitySide2 = baseVelocity - perpendicularVelocity + MathHelper::NextVector2(0.0f, 0.3f);

	exhaustPlume.particleManger->CreateParticle(
		exhaustPlume.particleTexture,
		position,
		velocitySide1,
		Vector4::One * alpha,
		50.0f,
		Vector2(0.4f, 0.7f));

	exhaustPlume.particleManger->CreateParticle(
		exhaustPlume.particleGlowTexture,
		position,
		velocitySide1,
		colour1 * alpha,
		50.0f,
		Vector2(0.4f, 0.7f));

	exhaustPlume.particleManger->CreateParticle(
		exhaustPlume.particleTexture,
		position,
		velocitySide2,
		Vector4::One * alpha,
		50.0f,
		Vector2(0.4f, 0.7f));

	exhaustPlume.particleManger->CreateParticle(
		exhaustPlume.particleGlowTexture,
		position,
		velocitySide2,
		colour1 * alpha,
		50.0f,
		Vector2(0.4f, 0.7f));	
}