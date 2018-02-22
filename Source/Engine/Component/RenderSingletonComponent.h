#pragma once
#include "Core/Component/Component.h"
#include "RenderSingletonComponent.gen.h"

class CameraComponent;

COMPONENT()
class RenderSingletonComponent : public Component
{
	GENERATED_BODY()

public:
	RenderSingletonComponent(Entity* entity) : Component(entity) {}

	CameraComponent* m_CurrentCamera = nullptr;
	int m_TestValue = 0;
};