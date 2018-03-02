#pragma once
#include "Core/Component/Component.h"
#include "BehaviourComponent.gen.h"
#include "Core/Resource/ScriptResource.h"

COMPONENT(Core)
class BehaviourComponent : public Component
{
	GENERATED_BODY()

public:
	BehaviourComponent(Entity* entity) : Component(entity) {}

	PROPERTY(Serialize)
	ScriptResource* m_Script = nullptr;
};