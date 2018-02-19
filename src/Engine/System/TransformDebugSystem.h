#pragma once
#include "Core/System/System.h"
#include "Engine/Component/TransformComponent.h"

class TransformDebugSystem : public System<const TransformComponent>
{
public:
	TransformDebugSystem(World* world) : SystemBase(world) {}

protected:
	void RunInternal(Entity* entity, const TransformComponent* transform);
};
