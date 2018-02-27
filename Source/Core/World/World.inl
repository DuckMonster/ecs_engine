template<class T>
inline T* World::GetSingletonComponent()
{
	if (!m_AnonymousEntity)
		m_AnonymousEntity = new Entity(0, "[ANON]");

	T* component = m_AnonymousEntity->GetComponent<T>();

	if (!component)
		component = m_AnonymousEntity->AddComponent<T>();

	return component;
}
