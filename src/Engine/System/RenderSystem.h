#include "Core/System/System.h"
#include "Engine/Component/RenderableComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/RenderSingletonComponent.h"

class RenderSystem : public System<const TransformComponent, const RenderableComponent>
{
public:
	RenderSystem(World* world) : SystemBase(world) {}

protected:
	void RunBegin() override;
	void RunInternal(Entity* entity, const TransformComponent* transform, const RenderableComponent* renderable) override;
	void RunEnd() override;

private:
	RenderSingletonComponent* m_RenderSingleton = nullptr;
};