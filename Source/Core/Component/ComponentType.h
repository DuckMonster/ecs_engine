#pragma once

_TYPE_DATABASE()
class ComponentType
{
public:
	typedef char id_t;
	static const id_t INVALID_ID;

public:
	_DATABASE_INIT_FUNC()
	static void InitializeTypes();

	static const ComponentType& FromId(id_t id);
	static const ComponentType& FromString(const char* name);

private:
	static std::map<std::string, ComponentType> s_StringTypeMap;
	static std::unordered_map<id_t, ComponentType> s_IdTypeMap;

	_DATABASE_REGISTER_FUNC()
	template<class TComp>
	static void RegisterType(const char* name, id_t id);

public:
	ComponentType() {}
	ComponentType(const char* name, id_t id) : m_Name(name), m_Id(id) {}

	id_t Id() const { return m_Id ; }
	const char* Name() const { return m_Name; }

	bool IsValid() const { return m_Id == INVALID_ID; }

private:
	const id_t m_Id = INVALID_ID;
	const char* m_Name = nullptr;
};

template<class TComp>
void ComponentType::RegisterType(const char* name, id_t id)
{
	Debug_Log("Component \"%s\" registered with id [ %d ]", name, id);

	ComponentType type(name, id);

	s_StringTypeMap.emplace(name, ComponentType(name, id));
	s_IdTypeMap.emplace(id, ComponentType(name, id));
}