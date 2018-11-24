#pragma once

#include "System.h"
#include "EntityManager.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"

class SystemRender : public System
{
public:
	SystemRender(EntityManager& manager) :
		System(manager, true)
	{
		ComponentTypeSet requiredComponents;
		requiredComponents.insert(RenderComponent::Type);
		requiredComponents.insert(TranslationComponent::Type);

		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void RenderEntity(Entity entity)
	{
		RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
		
		// TODO: Is this correct or should be do this on load of the texture?
		// Maybe look at this again during asset management.
		ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(render.resource);
		D3D11_TEXTURE2D_DESC description;
		texture->GetDesc(&description);
		float width = static_cast<float>(description.Width);
		float height = static_cast<float>(description.Height);

		render.spriteBatch.Draw(
			render.texture, 
			translation.position, 
			nullptr, 
			DirectX::Colors::White, 
			translation.orientation, 
			DirectX::SimpleMath::Vector2(width / 2, height / 2), 
			1.0f,
			DirectX::SpriteEffects::SpriteEffects_None,
			0);
	}
};