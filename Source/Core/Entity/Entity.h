#pragma once
#include <type_traits>
#include <tuple>
#include "Core/Tools/Name.h"
#include "Core/Component/ComponentType.h"
#include "EntityUtils.h"

class NamedArchive;
class World;

class Entity
{
public:
	Entity(World* world, const entity_id id, const FName& name);
	~Entity();

	const entity_id& GetId() const { return id; }
	const FName& GetName() const { return name; }

	template<class TComp>
	TComp* AddComponent() { return (TComp*)AddComponent(ComponentType::Get<TComp>()); }
	Component* AddComponent(const ComponentType& type);

	template<class TComp>
	TComp* GetComponent() const { return (TComp*)GetComponent(TComp::StaticType()); }
	Component* GetComponent(const ComponentType& type) const;

	template<class TComp>
	TComp* GetOrAddComponent() const { return (TComp*)GetOrAddComponent(TComp::StaticType()); }
	Component* GetOrAddComponent(const ComponentType& type);

	const std::vector<Component*>& GetAllComponents() const { return componentList; }

private:
	World* const world = nullptr;
	const entity_id id;
	FName name;

	std::vector<Component*> componentList;
};