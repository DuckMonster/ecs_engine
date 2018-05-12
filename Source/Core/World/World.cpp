#include "CorePCH.h"
#include "World.h"
#include "Core/Utils/GLUtils.h"
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
	m_SystemList.push_back(new CameraSystem(this));
	m_SystemList.push_back(new RenderSystem(this));
	m_SystemList.push_back(new BehaviourSystem(this));
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
	//-------------------- Print frame rate
	static float FRAME_TIMER = 0.f;

	if ((FRAME_TIMER += Time::FrameDelta()) > 1.f)
	{
		FRAME_TIMER = 0.f;
		//Debug_Log("Hello World, MS = %f", delta);
		//PrintWorld();
	}

#if DEBUG
	static float UPDATE_TIMER = 0.f;

	if ((UPDATE_TIMER += Time::FrameDelta()) > 0.5f)
	{
		//if (m_MapResource->HasChanged())
		//	LoadFromResource();

		//UPDATE_TIMER = 0.f;
	}
#endif

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
	Ensure(m_EntityLookup[id] == nullptr);
#endif

	Entity* newEntity = new Entity(id, name);
	m_EntityList.push_back(newEntity);
	m_EntityLookup[id] = newEntity;

	Debug_Log("Entity [%d] created: \"%s\"", id, name.c_str());

	return newEntity;
}

/**	Get Entity
*******************************************************************************/
Entity* World::GetEntity(entity_id id)
{
	return m_EntityLookup[id];
}

/**	Find Entity
*******************************************************************************/
Entity* World::FindEntity(const FName& name)
{
	for (Entity* entity : m_EntityList)
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
	Entity* entity = m_EntityLookup[id];

	if (!Ensure(entity))
		return;

	m_EntityLookup[id] = nullptr;

	std::vector<Entity*>::iterator it = std::find(m_EntityList.begin(), m_EntityList.end(), entity);
	if (Ensure(it != m_EntityList.end()))
		m_EntityList.erase(it);
}

/**	Map Resource Reloaded
*******************************************************************************/
void World::MapResourceReloaded(Resource* resource)
{
	Debug_Log("World::MapResourceReloaded");
	LoadFromResource();
}

/**	Load Map
*******************************************************************************/
void World::LoadMap(const char* path)
{
	if (!Ensure(m_MapResource == nullptr))
		return;

	m_MapResource = ResourceManager::GetInstance()->Load(path);
	m_MapResource->m_OnHotReloaded.BindObject(this, &World::MapResourceReloaded);

	LoadFromResource();
}

/**	Print World
*******************************************************************************/
void World::PrintWorld()
{
	Debug_Log("\n");
	Debug_Log("--- WORLD DUMP\n");

	for(Entity* entity : m_EntityList)
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
	for (ISystem* system : m_SystemList)
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

	NamedArchive::Source source = NamedArchive::Open(m_MapResource->GetPath());
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
