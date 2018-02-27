#pragma once
#include "Core/Meta/MetaData.h"

class Component;
class Entity;

_TYPE_DATABASE()
class ComponentType
{
public:
	typedef char id_t;
	static const id_t INVALID_ID;

private:
	//--------------------------------------------------- Make Functions
	typedef Component* (*MakeFunc)(Entity*);
	template<class T>
	static Component* DefaultMakeFunc(Entity* e) { return (Component*)new T(e); }

	//--------------------------------------------------- Template Lookup
	template<class TComp>
	class TemplateLookup
	{
	public:
		static ComponentType Type;
	};

	//--------------------------------------------------- Database
public:
	static void InitializeTypes();

	static const ComponentType& FromId(id_t id);
	static const ComponentType& FromString(const char* name);
	template<class TComp>
	static const ComponentType& Get() { return TemplateLookup<TComp>::Type; }

private:
	static std::map<std::string, ComponentType> s_StringTypeMap;
	static std::unordered_map<id_t, ComponentType> s_IdTypeMap;

	template<class TComp>
	static void RegisterType(const char* name, id_t id);

	//--------------------------------------------------- Class
public:
	ComponentType() {}
	ComponentType(const char* name, id_t id) : m_Name(name), m_Id(id) {}

	id_t Id() const { return m_Id ; }
	const char* Name() const { return m_Name; }

	bool IsValid() const { return m_Id != INVALID_ID; }
	Component* Make(Entity* entity) const { if (Ensure(m_MakeFunc)) return m_MakeFunc(entity); return nullptr; }

	bool operator==(const ComponentType& other) const { return other.m_Id == m_Id; }
	bool operator!=(const ComponentType& other) const { return !(*this == other); }

private:
	id_t m_Id = INVALID_ID;
	const char* m_Name = nullptr;
	MakeFunc m_MakeFunc = nullptr;
};

template<class TComp>
void ComponentType::RegisterType(const char* name, id_t id)
{
	static_assert(std::is_base_of<Component, TComp>::value, "Trying to register component with non-component template");

	Debug_Log("Component \"%s\" registered with id [ %d ]", name, id);

	ComponentType type(name, id);
	type.m_MakeFunc = ComponentType::DefaultMakeFunc<TComp>;

	s_StringTypeMap[name] = type;
	s_IdTypeMap[id] = type;
	TemplateLookup<TComp>::Type = type;
}

template<class TComp>
ComponentType ComponentType::TemplateLookup<TComp>::Type;