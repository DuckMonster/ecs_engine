#include "CorePCH.h"
#include "EmilTestComponent.h"
#include "Engine/Component/InputComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Core/Tools/Time.h"

using namespace glm;

/**	Start
*******************************************************************************/
void EmilTestComponent::Start()
{
	BIND_INPUT(&EmilTestComponent::UpdateInput);
}

/**	Update Input
*******************************************************************************/
void EmilTestComponent::UpdateInput(const InputState& state)
{
	TransformComponent* transform = GetSibling<TransformComponent>();

	vec3 direction(0.f);

	if (state.GetKey(Key::W))
		direction.z += 1.f;
	if (state.GetKey(Key::S))
		direction.z -= 1.f;
	if (state.GetKey(Key::D))
		direction.x += 1.f;
	if (state.GetKey(Key::A))
		direction.x -= 1.f;

	if (length2(direction) == 0.f)
		return;

	direction = normalize(direction);
	transform->Translate(direction * speed * Time::FrameDelta());
}