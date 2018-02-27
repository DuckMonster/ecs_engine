#pragma once
#include "../Entity/EntityUtils.h"

class Entity;
class ISystem;

class World 
{
public:
	World();
	~World();

	void DoFrame(float delta);
	const std::vector<Entity*>& GetEntities() const { return m_EntityList; }

	Entity* CreateEntity(FName& name);
	Entity* GetEntity(entity_id id);
	void DestroyEntity(entity_id id);
	void DestroyEntity(Entity* entity);

private:
	void PrintWorld();
	void RunSystems();

	void LoadMap(const char* path);

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
};

#include "World.inl"
