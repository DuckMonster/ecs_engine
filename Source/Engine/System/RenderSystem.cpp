#include "CorePCH.h"
#include "RenderSystem.h"
#include "Core/World/World.h"
#include "Core/Context/Context.h"
#include "Core/Rendering/Pipeline/RenderPipeline.h"
#include "Engine/Component/CameraComponent.h"

using namespace Rendering;

/**	Run Begin
*******************************************************************************/
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

	UpdateCameraMatrices();
	m_RenderSingleton->m_CurrentManifest.RenderList.clear();

	// Clear screen
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**	Run Internal
*******************************************************************************/
void RenderSystem::RunInternal(Entity* entity, const TransformComponent* transform, const RenderableComponent* renderable)
{
	MeshResource* mesh = renderable->m_Mesh;
	MaterialResource* material = renderable->m_Material;

	if (!mesh || !material || !material->IsValid())
		return;

	RenderableData data;
	data.Meshes = mesh->GetData();
	data.Material = material->GetData();

	data.ModelMatrix = transform->GetMatrix();
	data.ModelMatrixInv = inverse(transform->GetMatrix());
	data.NormalMatrix = transform->GetNormalMatrix();

	m_RenderSingleton->m_CurrentManifest.RenderList.push_back(data);
}

/**	Run End
*******************************************************************************/
void RenderSystem::RunEnd()
{
	IRenderPipeline* pipeline = IRenderPipeline::Get();
	pipeline->Render(m_RenderSingleton->m_CurrentManifest);
}

/**	Update Camera Matrices
*******************************************************************************/
void RenderSystem::UpdateCameraMatrices()
{
	using namespace glm;
	RenderManifest& manifest = m_RenderSingleton->m_CurrentManifest;
	CameraComponent* camera = m_RenderSingleton->m_CurrentCamera;

	if (!camera)
		return;

	TransformComponent* transform = camera->GetEntity()->GetComponent<TransformComponent>();

	if (!transform)
		return;

	//--------------------------------------------------- View matrix
	{
		// Get matrix from rotation
		quat rotation = transform->GetRotation();
		mat4 result = toMat4(rotation);

		// In this engine, X-axis is forward and Z-axis is right
		// So we have to swap X and Z, and also inverse Z to correspond to NDC
		vec3 f(result[0]),
			u(result[1]),
			r(result[2]);

		// Eye offset
		vec3 pos = transform->GetPosition();
		result = mat4(
			r.x, u.x, -f.x, 0.f,
			r.y, u.y, -f.y, 0.f,
			r.z, u.z, -f.z, 0.f,
			-dot(pos, r), -dot(pos, u), dot(pos, f), 1.f
		);

		//result = glm::lookAt(pos, vec3(), vec3(0.f, 1.f, 0.f));

		// Done!
		camera->m_ViewMatrix = result;
	}

	//--------------------------------------------------- Projection matrix
	{
		float ratio = (float)Context::Get()->GetWidth() / Context::Get()->GetHeight();

		if (camera->m_Perspective)
		{
			camera->m_ProjectionMatrix = glm::perspective(radians(camera->m_FieldOfView), ratio, camera->m_Near, camera->m_Far);
		}
		else
		{
			float height = camera->m_OrthoHeight;
			camera->m_ProjectionMatrix = glm::ortho(-ratio * height, ratio * height, -height, height, camera->m_Near, camera->m_Far);
		}
	}

	// Combine into camera matrix
	camera->m_CameraMatrix = camera->m_ProjectionMatrix * camera->m_ViewMatrix;

	// Fill out render manifest
	manifest.ProjectionMatrix = camera->m_ProjectionMatrix;
	manifest.ProjectionMatrixInv = inverse(camera->m_ProjectionMatrix);
	manifest.ViewMatrix = camera->m_ViewMatrix;
	manifest.ViewMatrixInv = inverse(camera->m_ViewMatrix);
	manifest.CameraMatrix = camera->m_CameraMatrix;
	manifest.CameraMatrixInv = inverse(camera->m_CameraMatrix);
}
