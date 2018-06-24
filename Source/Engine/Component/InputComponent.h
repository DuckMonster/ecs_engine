#pragma once
#include "Core/Component/Component.h"
#include "Core/Delegate/Delegate.h"
#include "Core/Input/InputState.h"
#include "InputComponent.gen.h"

#define BIND_INPUT(function) BIND_BASE(InputComponent, OnInput, function)

COMPONENT()
class InputComponent : public Component
{
	GENERATED_BODY()

public:
	InputComponent(Entity* entity) : Component(entity) {}
	Delegate<const InputState&> OnInput;
};