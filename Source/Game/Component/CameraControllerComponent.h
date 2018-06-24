#pragma once
#include "Core/Component/Component.h"
#include "CameraControllerComponent.gen.h"

class EmilTestComponent;

COMPONENT()
class CameraControllerComponent : public Component
{
	GENERATED_BODY()

public:
	CameraControllerComponent(Entity* entity) : Component(entity) {}

	PROPERTY(Serialize)
	EmilTestComponent* followTarget = nullptr;
};