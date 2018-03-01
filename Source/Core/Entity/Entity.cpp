#include "CorePCH.h"
#include "Entity.h"
#include "Core/Serialize/NamedArchive.h"
#include "Core/Component/Component.h"

Entity::Entity(const entity_id id, const FName& name) :
	m_Id(id),
	m_Name(name)
{
}

Entity::~Entity()
{
}

Component* Entity::AddComponent(const ComponentType& type)
{
	if (!Ensure(type.IsValid()))
		return nullptr;

	if (!Ensure(GetComponent(type) == nullptr))
		return nullptr;

	Component* comp = type.Make(this);
	comp->Initialize();

	m_ComponentList.push_back(comp);

	return comp;
}

Component* Entity::GetComponent(const ComponentType& type) const
{
	for (Component* comp : m_ComponentList)
	{
		if (comp->Type() == type)
			return comp;
	}

	return nullptr;
}