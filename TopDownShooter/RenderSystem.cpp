#include "stdafx.h"

#include "RenderSystem.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemRender::SystemRender(EntityManager& manager) :
    System(manager, true)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(RenderComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemRender::UpdateEntity(float dt, float totalTime, Entity entity)
{
	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
	
	if (render.paused)
	{
		return;
	}

	render.totalElapsed += dt;

	if (render.totalElapsed > render.timePerFrame)
	{
		render.frame++;
		render.frame = render.frame % render.frameCount;
		render.totalElapsed -= render.timePerFrame;
	}
}

void SystemRender::RenderEntity(Entity entity)
{
    RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

	int frameWidth = render.texture->GetWidth() / render.frameCount;

	RECT sourceRect;
	sourceRect.left = frameWidth * render.frame;
	sourceRect.top = 0;
	sourceRect.right = sourceRect.left + frameWidth;
	sourceRect.bottom = render.texture->GetHeight();

	render.spriteBatch.Draw(render.texture->GetSrv(),
		translation.position,
		&sourceRect,
		render.colour,
		translation.orientation,
		Vector2(static_cast<float>(frameWidth / 2),
			static_cast<float>(render.texture->GetHeight() / 2)),
		render.scale,
		SpriteEffects::SpriteEffects_None,
		0);
}