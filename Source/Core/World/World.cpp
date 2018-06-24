#include "CorePCH.h"
#include "World.h"
#include "Core/Tools/GLUtils.h"
#include "Core/Entity/Entity.h"
#include "Core/System/System.h"
#include "Core/Context/Context.h"
#include "Core/Serialize/NamedArchive.h"
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceManager.h"

#include "Engine/Engine.h"
#include "Engine/Component/RenderSingletonComponent.h"

using namespace glm;

/**	Constructor
*******************************************************************************/
World::World()
{
	systemList.push_back(new InputSystem(this));
	systemList.push_back(new BehaviourSystem(this));
	systemList.push_back(new CameraSystem(this));
	systemList.push_back(new RenderSystem(this));
}

/**	Destructor
*******************************************************************************/
World::~World()
{
}

/**	Do Frame
*******************************************************************************/
void World::DoFrame()
{
	// Init components
	StartPendingComponents();

	// Run systems
	RunSystems();
}

/**	Create Entity
*******************************************************************************/
Entity* World::CreateEntity(FName& name)
{
	entity_id id = m_LastEntityId++;

	// Id overflow
	Ensure(id != MAX_ENTITY);

#if DEBUG
	Ensure(entityLookup[id] == nullptr);
#endif

	Entity* newEntity = new Entity(this, id, name);
	entityList.push_back(newEntity);
	entityLookup[id] = newEntity;

	Debug_Log("Entity [%d] created: \"%s\"", id, name.c_str());

	return newEntity;
}

/**	Get Entity
*******************************************************************************/
Entity* World::GetEntity(entity_id id)
{
	return entityLookup[id];
}

/**	Find Entity
*******************************************************************************/
Entity* World::FindEntity(const FName& name)
{
	for (Entity* entity : entityList)
	{
		if (entity->GetName() == name)
			return entity;
	}

	return nullptr;
}

/**	Destroy Entity
*******************************************************************************/
void World::DestroyEntity(Entity* entity) { DestroyEntity(entity->GetId()); }
void World::DestroyEntity(entity_id id)
{
	Entity* entity = entityLookup[id];

	if (!Ensure(entity))
		return;

	entityLookup[id] = nullptr;

	std::vector<Entity*>::iterator it = std::find(entityList.begin(), entityList.end(), entity);
	if (Ensure(it != entityList.end()))
		entityList.erase(it);
}

/**	Map Resource Reloaded
*******************************************************************************/
void World::MapResourceReloaded()
{
	LoadFromResource();
}

/**	Add Pending Component
*******************************************************************************/
void World::AddPendingComponent(Component* component)
{
	pendingComponentList.push_back(component);
}

/**	Load Map
*******************************************************************************/
void World::LoadMap(const char* path)
{
	if (!Ensure(m_MapResource == nullptr))
		return;

	m_MapResource = ResourceManager::GetInstance()->Load<Resource>(path);
	m_MapResource->m_OnHotReloaded.BindObject(this, &World::MapResourceReloaded);

	LoadFromResource();
}

/**	Start Pending Components
*******************************************************************************/
void World::StartPendingComponents()
{
	for(Component* component : pendingComponentList)
		component->Start();

	pendingComponentList.clear();
}

/**	Print World
*******************************************************************************/
void World::PrintWorld()
{
	Debug_Log("\n");
	Debug_Log("--- WORLD DUMP\n");

	for(Entity* entity : entityList)
	{
		Debug_Log(entity->GetName().c_str());

		for (Component* comp : entity->GetAllComponents())
			comp->DebugPrint();
	}

	Debug_Log("--- WORLD END");
}

/**	Run Systems
*******************************************************************************/
void World::RunSystems()
{
	for (ISystem* system : systemList)
	{
		system->Run();
	}
}

/**	Load Map
*******************************************************************************/
void World::LoadFromResource()
{
	if (m_MapResource == nullptr)
		return;

	NamedArchive::Source source = NamedArchive::Open(m_MapResource->GetFile().GetPath());
	NamedArchive archive(source);

	const char* mapName = nullptr;
	archive.Serialize("name", mapName);

	if (!Ensure(mapName))
		return;

	Debug_Log("Loading map \"%s\"...", mapName);

	{
		//--------------------------------------------------- Read Entities
		NamedArchive entityListArchive = archive.Push("entities");
		Ensure(entityListArchive.IsArray());

		for (::uint32 i = 0; i < entityListArchive.ArraySize(); ++i)
		{
			// Get name and create
			NamedArchive entityArchive = entityListArchive.Push(i);
			const char* entityName = nullptr;
			if (!Ensure(entityArchive.Serialize("name", entityName)))
				continue;

			// Try to find and modify, otherwise it's a new entity!
			Entity* entity = FindEntity(FName(entityName));

			if (!entity)
				entity = CreateEntity(FName(entityName));

			// Read components
			NamedArchive componentListArchive = entityArchive.Push("components");

			Ensure(componentListArchive.IsArray());
			for (::uint32 i = 0; i < componentListArchive.ArraySize(); ++i)
			{
				NamedArchive componentArchive = componentListArchive.Push(i);

				// Get type
				const char* componentType = nullptr;
				if (!Ensure(componentArchive.Serialize("type", componentType)))
					continue;

				ComponentType type = ComponentType::FromString(componentType);
				if (!Ensure(type.IsValid()))
					continue;

				// Find or create this component
				Component* comp = entity->GetComponent(type);

				if (!comp)
					comp = entity->AddComponent(type);

				comp->Serialize(componentArchive);
			}
		}

		//--------------------------------------------------- Read camera
		{
			const char* cameraEntityString = nullptr;
			archive.Serialize("camera", cameraEntityString);

			if (Ensure(cameraEntityString))
			{
				Entity* camera = FindEntity(FName(cameraEntityString));
				CameraComponent* cameraComponent = nullptr;
				if (Ensure(camera))
					cameraComponent = camera->GetComponent<CameraComponent>();

				GetSingletonComponent<RenderSingletonComponent>()->m_CurrentCamera = cameraComponent;
			}
		}
	}

	PrintWorld();
}

/**	Get Singleton Component
*******************************************************************************/
Component* World::GetSingletonComponent(ComponentType type)
{
	if (!m_AnonymousEntity)
		m_AnonymousEntity = new Entity(this, 0, "[ANON]");

	Component* component = m_AnonymousEntity->GetComponent(type);

	if (!component)
		component = m_AnonymousEntity->AddComponent(type);

	return component;
}