#pragma once

class Entity;
class ISystem;

class World 
{
public:
	World();
	~World();

	void DoFrame(float delta);
	const std::vector<Entity*>& GetEntities() const { return m_EntityList; }


private:
	void PrintWorld();
	void RunSystems();

	std::vector<Entity*> m_EntityList;
	std::vector<ISystem*> m_SystemList;

	//--------------------------------------------------- Singleton stuff
public:
	template<class T>
	T* GetSingletonComponent();

private:
	Entity* m_AnonymousEntity = nullptr;
};

#include "World.inl"
