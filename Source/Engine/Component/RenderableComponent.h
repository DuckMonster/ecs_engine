#pragma once
#include "Core/Meta/MetaData.h"
#include "Core/Component/Component.h" 
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"
#include "Engine/Rendering/RenderTypes.h"
#include "RenderableComponent.gen.h"

COMPONENT(Editor)
class RenderableComponent : public Component
{
	GENERATED_BODY()

public:
	RenderableComponent(Entity* entity);

	RenderFlag m_Flags;

	PROPERTY(Serialize)
	MeshResource* m_Mesh = nullptr;

	PROPERTY(Serialize)
	MaterialResource* m_Material = nullptr;
};