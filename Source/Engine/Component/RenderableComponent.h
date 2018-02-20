#pragma once
#include "Core/Component/Component.h" 
#include "Engine/Rendering/RenderTypes.h"

COMPONENT(Editor)
class RenderableComponent : public Component
{
public:
	RenderableComponent(Entity* entity) : Component(entity) {}

	PROPERTY(EditorEditable)
	GLuint m_VertexObject;
	PROPERTY(EditorEditable, Replicated)
	GLuint m_ShaderProgram;

	RenderFlag m_Flags;
	GLenum m_DrawMode;
	uint m_DrawCount;
	bool m_UsingElements;
};