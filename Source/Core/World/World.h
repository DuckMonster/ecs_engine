#pragma once
#include "Core/Entity/EntityUtils.h"
#include "Core/Component/ComponentType.h"

class Entity;
class Component;
class ISystem;
class Resource;

class World 
{
public:
	World();
	~World();

	void DoFrame();

	// Entity stuff
	const std::vector<Entity*>& GetEntities() const { return entityList; }

	Entity* CreateEntity(FName& name);
	Entity* GetEntity(entity_id id);
	Entity* FindEntity(const FName& name);
	void DestroyEntity(entity_id id);
	void DestroyEntity(Entity* entity);

	// Resource loading
	void LoadMap(const char* path);
	void MapResourceReloaded();

private:
	void LoadFromResource();

	// Pending components (for next-frame start-up calling)
public:
	void AddPendingComponent(Component* component);
private:
	void StartPendingComponents();

private:
	// Debug print
	void PrintWorld();
	void RunSystems();

	//--------------------------------------------------- Singleton stuff
public:
	template<class T>
	T* GetSingletonComponent() { return (T*)GetSingletonComponent(T::StaticType()); }
	Component* GetSingletonComponent(ComponentType type);

private:
	Entity* m_AnonymousEntity = nullptr;

	//--------------------------------------------------- Entity Management
private:
	entity_id m_LastEntityId = 0;

	std::vector<Entity*> entityList;
	std::unordered_map<entity_id, Entity*> entityLookup;
	std::vector<ISystem*> systemList;
	std::vector<Component*> pendingComponentList;

	//--------------------------------------------------- Map resource
private:
	Resource* m_MapResource = nullptr;
};