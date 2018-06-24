#pragma once
#include "Core/Component/Component.h"
#include "EmilTestComponent.gen.h"

COMPONENT()
class VelocityComponent : public Component
{
	GENERATED_BODY()

public:
	VelocityComponent(Entity* entity) : Component(entity) {}

	void Start() override;

private:
	void UpdateVelocity();

	PROPERTY(Serialize)
	glm::vec3 velocity;

	PROPERTY(Serialize)
	float friction;
};