#pragma once
#include "Core/Component/Component.h"
#include "TransformComponent.gen.h"

COMPONENT(Core)
class TransformComponent : public Component
{
	GENERATED_BODY()

public:
	TransformComponent(Entity* entity) : Component(entity) {}

	void Translate(const glm::vec3& delta) { SetPosition(m_Position + delta); }
	void SetPosition(const glm::vec3& position) { m_Position = position; }
	const glm::vec3& GetPosition() const { return m_Position; }

	void SetScale(const glm::vec3& scale) { m_Scale = scale; }
	const glm::vec3& GetScale() const { return m_Scale; }

	void Rotate(const glm::quat& rotation) { SetRotation(m_Rotation * rotation); }
	void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; }
	const glm::quat& GetRotation() const { return m_Rotation; }

	void LookAt(const glm::vec3& lookAtLocation, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	glm::mat4 GetMatrix() const;
	glm::mat4 GetNormalMatrix() const;

private:
	PROPERTY(Serialize)
	glm::vec3 m_Position = glm::vec3(0.f);

	PROPERTY(Serialize)
	glm::vec3 m_Scale = glm::vec3(1.f);

	PROPERTY(Serialize)
	glm::quat m_Rotation = glm::quat(1.f, 0.f, 0.f, 0.f);


	mutable glm::mat4 m_LocalMatrix = glm::mat4(1.f);
	mutable glm::mat4 m_LocalMatrixInv = glm::mat4(1.f);
};