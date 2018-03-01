#pragma once
#include "Core/Meta/MetaData.h"
#include "Core/Component/Component.h" 
#include "Core/Resource/MeshResource.h"
#include "Engine/Rendering/RenderTypes.h"
#include "RenderableComponent.gen.h"

COMPONENT(Editor)
class RenderableComponent : public Component
{
	GENERATED_BODY()

public:
	RenderableComponent(Entity* entity);

	GLuint m_ShaderProgram;
	RenderFlag m_Flags;

	PROPERTY(Serialize)
	MeshResource* m_Mesh = nullptr;
};