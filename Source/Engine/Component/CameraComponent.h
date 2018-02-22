#pragma once
#include "Core/Component/Component.h"
#include "CameraComponent.gen.h"

COMPONENT()
class CameraComponent : public Component 
{
	GENERATED_BODY()

public:
	CameraComponent(Entity* entity) : Component(entity), m_ProjectionMatrix(1.f) {}

	PROPERTY()
	float m_HerpValue = 5.f;
	PROPERTY()
	int m_IntValue = 200;
	PROPERTY()
	std::string m_StringValue = "Hej hej! :)";

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_CameraMatrix;
};