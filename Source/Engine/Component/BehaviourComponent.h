#pragma once
#include "Core/Component/Component.h"
#include "BehaviourComponent.gen.h"
#include "Core/Resource/ScriptResource.h"

#define BIND_BEHAVIOUR(function) BIND_BASE(BehaviourComponent, OnUpdate, function)

COMPONENT(Core)
class BehaviourComponent : public Component
{
	GENERATED_BODY()

public:
	BehaviourComponent(Entity* entity) : Component(entity) {}
	Delegate<> OnUpdate;
};