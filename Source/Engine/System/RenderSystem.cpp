#include "CorePCH.h"
#include "RenderSystem.h"
#include "Core/World/World.h"
#include "Engine/Component/CameraComponent.h"

void RenderSystem::RunBegin()
{
	m_RenderSingleton = GetWorld()->GetSingletonComponent<RenderSingletonComponent>();

	if (!m_RenderSingleton->m_CurrentCamera)
	{
		for (Entity* entity : GetWorld()->GetEntities())
		{
			m_RenderSingleton->m_CurrentCamera = entity->GetComponent<CameraComponent>();
			if (m_RenderSingleton->m_CurrentCamera != nullptr)
				break;
		}
	}

	glEnable(GL_DEPTH_TEST);

	// Clear screen
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::RunInternal(Entity* entity, const TransformComponent* transform, const RenderableComponent* renderable)
{
	MeshResource* mesh = renderable->m_Mesh;
	if (!mesh)
		return;

	glBindVertexArray(mesh->GetVAO());
	glUseProgram(renderable->m_ShaderProgram);

	GLuint u_Camera = glGetUniformLocation(renderable->m_ShaderProgram, "u_Camera");
	glUniformMatrix4fv(u_Camera, 1, false, glm::value_ptr(m_RenderSingleton->m_CurrentCamera->m_CameraMatrix));

	GLuint u_Model = glGetUniformLocation(renderable->m_ShaderProgram, "u_Model");
	glUniformMatrix4fv(u_Model, 1, false, glm::value_ptr(transform->GetMatrix()));

	if (mesh->GetUsingElements())
		glDrawElements(mesh->GetDrawMode(), mesh->GetDrawCount(), GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(mesh->GetDrawMode(), 0, mesh->GetDrawCount());

	glBindVertexArray(0);
}

void RenderSystem::RunEnd()
{

}
