#pragma once
#include "Core/Component/Component.h"

class CameraComponent;

class RenderSingletonComponent : public Component
{
public:
	RenderSingletonComponent(Entity* entity) : Component(entity) {}

	CameraComponent* m_CurrentCamera = nullptr;
	int m_TestValue = 0;
};