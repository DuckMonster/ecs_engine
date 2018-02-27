#pragma once
#include "Core/Meta/MetaData.h"
#include "Core/Component/Component.h" 
#include "Engine/Rendering/RenderTypes.h"
#include "RenderableComponent.gen.h"

COMPONENT(Editor)
class RenderableComponent : public Component
{
	GENERATED_BODY()

public:
	RenderableComponent(Entity* entity) : Component(entity) {}

	GLuint m_VertexObject;
	GLuint m_ShaderProgram;

	RenderFlag m_Flags;
	GLenum m_DrawMode;

	uint32 m_DrawCount;
	bool m_UsingElements;
};