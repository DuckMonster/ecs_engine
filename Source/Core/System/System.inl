#include <type_traits>

template<class... TComponents>
inline void System<TComponents...>::Run()
{
	RunBegin();

	const std::vector<Entity*>& EntityList = m_World->GetEntities();
	for (Entity* entity : EntityList)
	{
		RunFor<TComponents...>(entity);
	}

	RunEnd();
}

template<class... TComponents>
template<class... TRead>
inline void System<TComponents...>::RunFor(Entity* entity, TRead*... readComponents)
{
	RunInternal(entity, readComponents...);
}

template<class... TComponents>
template<class TToRead, class... TLeft, class... TRead>
inline void System<TComponents...>::RunFor(Entity* entity, TRead*... readComponents)
{
	TToRead* comp = entity->GetComponent<std::remove_const<TToRead>::type>();
	if (!comp)
		return;

	RunFor<TLeft...>(entity, readComponents..., comp);
}
