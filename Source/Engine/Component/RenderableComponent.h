#pragma once
#include "Core/Meta/MetaData.h"
#include "Core/Component/Component.h" 
#include "Engine/Rendering/RenderTypes.h"

COMPONENT(Editor)
class RenderableComponent : public Component
{
public:
	RenderableComponent(Entity* entity) : Component(entity) {}

	PROPERTY(EditorEditable, Serialize)
	GLuint m_VertexObject;
	PROPERTY(EditorEditable, Replicated, Serialize)
	GLuint m_ShaderProgram;

	RenderFlag m_Flags;
	GLenum m_DrawMode;
	uint m_DrawCount;
	bool m_UsingElements;
};