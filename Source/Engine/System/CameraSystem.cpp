#include "CorePCH.h"
#include "CameraSystem.h"
#include "Core/World/World.h"
#include "Core/Utils/Time.h"
#include "Core/Context/Context.h"
#include <glm/gtc/matrix_access.hpp>

using namespace glm;

void CameraSystem::RunBegin()
{
	m_RenderSingleton = GetWorld()->GetSingletonComponent<RenderSingletonComponent>();

	// Clear screen
	glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void CameraSystem::RunInternal(Entity* entity, TransformComponent* transform, CameraComponent* camera)
{
	if (camera != m_RenderSingleton->m_CurrentCamera)
		return;

	transform->SetPosition(vec3(
		sin(FTime::TotalElapsed()), 
		1.f,
		cos(FTime::TotalElapsed())
	));
	transform->LookAt(vec3());

	{
		// Get matrix from rotation
		quat rotation = transform->GetRotation();
		mat4 result = toMat4(rotation);

		// In this engine, X-axis is forward and Z-axis is right
		// So we have to swap X and Z, and also inverse Z to correspond to NDC
		vec3 f(-result[0]),
			u(result[1]),
			r(result[2]);

		// Eye offset
		vec3 pos = transform->GetPosition();
		result = mat4(
			r.x, u.x, f.x, 0.f,
			r.y, u.y, f.y, 0.f,
			r.z, u.z, f.z, 0.f,
			-dot(pos, r), -dot(pos, u), dot(pos, f), 1.f
		);

		// Done!
		camera->m_ViewMatrix = result;
	}

	float ratio = (float)Context::GetInstance()->width / (float)Context::GetInstance()->height;
	camera->m_ProjectionMatrix = glm::ortho(-ratio * 5.f, ratio * 5.f, -5.f, 5.f, -10.f, 10.f);

	camera->m_CameraMatrix = camera->m_ProjectionMatrix * camera->m_ViewMatrix;
}