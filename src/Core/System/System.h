#pragma once
#include "Core/World/World.h"
#include "Core/Entity/Entity.h"

class ISystem
{
public:
	virtual void Run() = 0;
};

template<class... TComponents>
class System : public ISystem
{
protected:
	typedef System<TComponents...> SystemBase;

public:
	System(World* world) : m_World(world) {}
	virtual ~System() {}

	void Run();
	World* GetWorld() const { return m_World; }

protected:
	virtual void RunBegin() {}
	virtual void RunInternal(Entity* entity, TComponents*...) = 0;
	virtual void RunEnd() {}

private:
	template<class... TRead>
	void RunFor(Entity* entity, TRead*... readComponents);

	template<class TToRead, class... TLeft, class... TRead>
	void RunFor(Entity* entity, TRead*... readComponents);

	World* const m_World;
};

#include "System.inl"