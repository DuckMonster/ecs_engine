#pragma once
#include "ComponentType.h"

class Entity;

class Component
{
public:
	Component(Entity* entity) : m_Entity(entity) {}
	virtual ~Component() {}

	const ComponentType& Type() const { return m_Type; }
	static const ComponentType& StaticType()
	{
		static ComponentType INVALID_TYPE;
		return INVALID_TYPE;
	}

protected:
	virtual void Initialize();
	ComponentType m_Type = ComponentType();

private:
	Entity* const m_Entity;
};