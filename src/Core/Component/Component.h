#pragma once

class Entity;

class Component
{
public:
	Component(Entity* entity) : m_Entity(entity) {}
	virtual ~Component() {}

private:
	Entity* const m_Entity;
};