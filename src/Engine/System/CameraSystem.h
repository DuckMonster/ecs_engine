#include "Core/System/System.h"
#include "Engine/Component/CameraComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/RenderSingletonComponent.h"

class CameraSystem : public System<TransformComponent, CameraComponent>
{
public:
	CameraSystem(World* world) : SystemBase(world) {}

protected:
	void RunBegin() override;
	void RunInternal(Entity* entity, TransformComponent* transform, CameraComponent* render) override;

private:
	RenderSingletonComponent* m_RenderSingleton = nullptr;
};