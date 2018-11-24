#pragma once

#include "Component.h"
#include "SpriteBatch.h"

struct RenderComponent : public Component 
{
	static const ComponentType Type;

	DirectX::SpriteBatch& spriteBatch;
	ID3D11ShaderResourceView* tex;

	RenderComponent(DirectX::SpriteBatch& sb, ID3D11ShaderResourceView* aTex) : 
		spriteBatch(sb), 
		tex(aTex) { }
};

const ComponentType RenderComponent::Type = 2;

#include "System.h"
#include "Manager.h"
#include "PositionComponent.h"

class SystemRender : public System
{
public:
	SystemRender(Manager& manager) :
		System(manager, true)
	{
		ComponentTypeSet requiredComponents;
		requiredComponents.insert(RenderComponent::Type);
		requiredComponents.insert(PositionComponent::Type);

		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void RenderEntity(Entity entity)
	{
		RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
		PositionComponent& position = m_manager.GetComponentStore<PositionComponent>().Get(entity);

		render.spriteBatch.Draw(render.tex, DirectX::XMFLOAT2(position.x, position.y), DirectX::Colors::White);
	}
};