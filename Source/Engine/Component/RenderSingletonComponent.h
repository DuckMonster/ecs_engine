#pragma once
#include "Core/Component/Component.h"
#include "Core/Rendering/Pipeline/RenderPipeline.h"
#include "RenderSingletonComponent.gen.h"

class CameraComponent;

COMPONENT()
class RenderSingletonComponent : public Component
{
	GENERATED_BODY()

public:
	RenderSingletonComponent(Entity* entity) : Component(entity) {}

	CameraComponent* m_CurrentCamera = nullptr;
	Rendering::RenderManifest m_CurrentManifest;
};