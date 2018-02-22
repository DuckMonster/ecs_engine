template<class TComp, class ...TArgs>
inline TComp* Entity::AddComponent(TArgs ...args)
{
	static_assert(std::is_base_of<Component, TComp>::value, "Trying to add component with non-component template");

	TComp* component = new TComp(this, args...);
	component->Initialize();
	m_ComponentList.push_back(component);

	return component;
}

template<class TComp>
inline TComp* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, TComp>::value, "Trying to get component with non-component template");

	for (Component* comp : m_ComponentList)
	{
		TComp* castedComponent = dynamic_cast<TComp*>(comp);
		if (castedComponent)
		{
			return castedComponent;
		}
	}

	return nullptr;
}