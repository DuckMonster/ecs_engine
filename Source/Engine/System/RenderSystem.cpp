#include "CorePCH.h"
#include "RenderSystem.h"
#include "Core/World/World.h"
#include "Engine/Component/CameraComponent.h"

void RenderSystem::RunBegin()
{
	m_RenderSingleton = GetWorld()->GetSingletonComponent<RenderSingletonComponent>();

	// Clear screen
	glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void RenderSystem::RunInternal(Entity* entity, const TransformComponent* transform, const RenderableComponent* renderable)
{
	glBindVertexArray(renderable->m_VertexObject);
	glUseProgram(renderable->m_ShaderProgram);

	GLuint u_Camera = glGetUniformLocation(renderable->m_ShaderProgram, "u_Camera");
	glUniformMatrix4fv(u_Camera, 1, false, glm::value_ptr(m_RenderSingleton->m_CurrentCamera->m_CameraMatrix));

	GLuint u_Model = glGetUniformLocation(renderable->m_ShaderProgram, "u_Model");
	glUniformMatrix4fv(u_Model, 1, false, glm::value_ptr(transform->GetMatrix()));

	if (renderable->m_UsingElements)
		glDrawElements(renderable->m_DrawMode, renderable->m_DrawCount, GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(renderable->m_DrawMode, 0, renderable->m_DrawCount);

	glBindVertexArray(0);
}

void RenderSystem::RunEnd()
{

}
