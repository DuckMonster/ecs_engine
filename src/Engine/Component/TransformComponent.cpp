#include "CorePCH.h"
#include "TransformComponent.h"
using namespace glm;

void TransformComponent::LookAt(const glm::vec3& lookAtLocation, const glm::vec3& up /*= glm::vec3(0.f, 1.f, 0.f)*/)
{
	mat3 result(uninitialize);

	vec3 f = normalize(lookAtLocation - m_Position),
		r = normalize(cross(f, up)),
		u = cross(r, f);

	result[0] = f;
	result[1] = u;
	result[2] = r;

	m_Rotation = quat(result);

	Debug_Log("LookAt {%f, %f, %f} -> {%f, %f, %f} = {%f | %f, %f, %f}",
		m_Position.x, m_Position.y, m_Position.z,
		lookAtLocation.x, lookAtLocation.y, lookAtLocation.z,
		m_Rotation.w, m_Rotation.x, m_Rotation.y, m_Rotation.z
	);
}

glm::vec3 TransformComponent::GetForward() const
{
	return normalize(glm::vec3(GetMatrix()[0]));
}

glm::vec3 TransformComponent::GetUp() const
{
	return normalize(glm::vec3(GetMatrix()[1]));
}

glm::vec3 TransformComponent::GetRight() const
{
	return normalize(glm::vec3(GetMatrix()[2]));
}

glm::mat4 TransformComponent::GetMatrix() const
{
	mat4 scaleMat = scale( mat4( 1.f ), m_Scale );
	
	m_LocalMatrix = toMat4(m_Rotation) * scaleMat;
	m_LocalMatrix[3] = vec4( m_Position, 1.f );

	m_LocalMatrixInv = inverse( m_LocalMatrix );

	return m_LocalMatrix;
}
