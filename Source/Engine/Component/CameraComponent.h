#pragma once
#include "Core/Component/Component.h"
#include "CameraComponent.gen.h"

COMPONENT()
class CameraComponent : public Component 
{
	GENERATED_BODY()

public:
	CameraComponent(Entity* entity) : Component(entity), m_ProjectionMatrix(1.f) {}

	PROPERTY(Serialize)
	float m_Far = 1.f;
	PROPERTY(Serialize)
	float m_Near = 0.f;
	PROPERTY(Serialize)
	bool m_Perspective = false;
	PROPERTY(Serialize)
	float m_OrthoHeight = 5.f;
	PROPERTY(Serialize)
	float m_FieldOfView = 45.f;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_CameraMatrix;
	glm::mat4 m_ProjectionMatrix;
};