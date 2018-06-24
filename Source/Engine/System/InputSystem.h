#include "Core/System/System.h"
#include "Core/Input/InputState.h"

class InputComponent;

class InputSystem : public System<InputComponent>
{
public:
	InputSystem(World* world) : SystemBase(world) {}

protected:
	void RunBegin() override;
	void RunInternal(Entity* entity, InputComponent* input) override;

	const InputState* inputState;
};