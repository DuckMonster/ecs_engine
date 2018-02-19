#pragma once
#include "Core/Component/Component.h"

class CameraComponent : public Component 
{
public:
	CameraComponent(Entity* entity) : Component(entity), m_ProjectionMatrix(1.f) {}

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_CameraMatrix;
};