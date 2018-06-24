#pragma once
#include "Core/Component/Component.h"
#include "EmilTestComponent.gen.h"

class InputState;

COMPONENT()
class EmilTestComponent : public Component
{
	GENERATED_BODY()

public:
	EmilTestComponent(Entity* entity) : Component(entity) {};

	void Start() override;

private:
	void UpdateInput(const InputState& state);

	PROPERTY(Serialize)
	float speed = 0.f;
};