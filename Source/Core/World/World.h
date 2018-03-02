#pragma once
#include "../Entity/EntityUtils.h"

class Entity;
class ISystem;
class Resource;

class World 
{
public:
	World();
	~World();

	void DoFrame();
	const std::vector<Entity*>& GetEntities() const { return m_EntityList; }

	Entity* CreateEntity(FName& name);
	Entity* GetEntity(entity_id id);
	Entity* FindEntity(const FName& name);
	void DestroyEntity(entity_id id);
	void DestroyEntity(Entity* entity);

	void LoadMap(const char* path);
	void MapResourceReloaded(Resource* resource);

private:
	void PrintWorld();
	void RunSystems();

	void LoadFromResource();

	//--------------------------------------------------- Singleton stuff
public:
	template<class T>
	T* GetSingletonComponent();

private:
	Entity* m_AnonymousEntity = nullptr;

	//--------------------------------------------------- Entity Management
private:
	entity_id m_LastEntityId = 0;

	std::vector<Entity*> m_EntityList;
	std::unordered_map<entity_id, Entity*> m_EntityLookup;
	std::vector<ISystem*> m_SystemList;

	//--------------------------------------------------- Map resource
private:
	Resource* m_MapResource = nullptr;
};

#include "World.inl"
