#include "CorePCH.h"
#include "InputSystem.h"
#include "Core/Context/Context.h"
#include "Engine/Component/InputComponent.h"

void InputSystem::RunBegin()
{
	inputState = &Context::Get()->GetInputState();
}

void InputSystem::RunInternal( Entity* entity, InputComponent* input )
{
	input->OnInput(*inputState);
}