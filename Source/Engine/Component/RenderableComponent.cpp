#include "CorePCH.h"
#include "RenderableComponent.h"
#include "Core/Utils/GLUtils.h"

RenderableComponent::RenderableComponent(Entity* entity) : Component(entity)
{
	m_ShaderProgram = GLUtils::CreateShaderFromFile("Resource/Shader/test");
}
