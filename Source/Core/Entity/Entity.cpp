#include "CorePCH.h"
#include "Entity.h"
#include "Core/Serialize/NamedArchive.h"
#include "Core/Component/Component.h"
#include "Core/World/World.h"

/**	Constructor
*******************************************************************************/
Entity::Entity(World* world, const entity_id id, const FName& name) :
	id(id), name(name), world(world)
{
}

/**	Destructor
*******************************************************************************/
Entity::~Entity()
{
}

/**	Add Component
*******************************************************************************/
Component* Entity::AddComponent(const ComponentType& type)
{
	if (!Ensure(type.IsValid()))
		return nullptr;

	if (!Ensure(GetComponent(type) == nullptr))
		return nullptr;

	Component* comp = type.Make(this);
	comp->InitializeInternal();
	componentList.push_back(comp);

	world->AddPendingComponent(comp);

	return comp;
}

/**	Get Component
*******************************************************************************/
Component* Entity::GetComponent(const ComponentType& type) const
{
	for (Component* comp : componentList)
	{
		if (comp->Type() == type)
			return comp;
	}

	return nullptr;
}

/**	Get Or Add Component
*******************************************************************************/
Component* Entity::GetOrAddComponent(const ComponentType& type)
{
	Component* comp = GetComponent(type);

	if (comp)
		return comp;

	return AddComponent(type);
}
