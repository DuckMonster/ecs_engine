#pragma once
#include <type_traits>
#include <tuple>
#include "Core/Utils/Name.h"
#include "Core/Component/ComponentType.h"
#include "EntityUtils.h"

class NamedArchive;

class Entity
{
public:
	Entity(const entity_id id, const FName& name);
	~Entity();

	const entity_id& GetId() const { return m_Id; }
	const FName& GetName() const { return m_Name; }

	template<class TComp>
	TComp* AddComponent() { return (TComp*)AddComponent(ComponentType::Get<TComp>()); }
	Component* AddComponent(const ComponentType& type);
	template<class TComp>
	TComp* GetComponent() const { return (TComp*)GetComponent(ComponentType::Get<TComp>()); }
	Component* GetComponent(const ComponentType& type) const;

	const std::vector<Component*>& GetAllComponents() const { return m_ComponentList; }

private:
	const entity_id m_Id;
	FName m_Name;

	std::vector<Component*> m_ComponentList;
};

#include "Entity.inl"