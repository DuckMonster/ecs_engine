#pragma once
#include <type_traits>
#include <tuple>
#include "Core/Utils/Name.h"

class Component;

class Entity
{
public:
	Entity(const FName& Name);
	~Entity();

	const FName& GetName() const { return m_Name; }

	template<class TComp, class... TArgs>
	TComp* AddComponent(TArgs... args);
	template<class TComp>
	TComp* GetComponent() const;

	const std::vector<Component*>& GetAllComponents() const { return m_ComponentList; }

private:
	FName m_Name;
	std::vector<Component*> m_ComponentList;
};

#include "Entity.inl"