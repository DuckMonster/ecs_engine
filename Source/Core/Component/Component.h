#pragma once
#include "ComponentType.h"
#include "Core/Tools/StringUtils.h"
#include "PropertyUtils.h"
#include "Core/Meta/MetaData.h"

class Entity;
class NamedArchive;

#define BIND_BASE(compName, delName, funcName) \
do\
{\
	compName* comp = GetOrAddSibling<compName>();\
	comp->delName.BindObject(this, funcName);\
} while(false)

class Component
{
	struct Property
	{
		typedef std::string(*StringFunc)(const void*);

		Property(const char* name, const void* value, StringFunc stringFunc) : m_Name(name), m_Value(value), m_StringFunc(stringFunc) {}
		const char* const m_Name;
		const void* const m_Value;
		StringFunc const m_StringFunc;
	};

public:
	Component(Entity* entity) : m_Entity(entity) {}
	virtual ~Component() {}

	virtual void InitializeInternal() {}
	virtual void Start() {}

	const ComponentType& Type() const { return m_Type; }
	static const ComponentType& StaticType()
	{
		static ComponentType INVALID_TYPE;
		return INVALID_TYPE;
	}

	Entity* GetEntity() const { return m_Entity; }

	void DebugPrint();
	virtual void Serialize(NamedArchive& archive) {}
	virtual void OnSerialized() {}

	template<typename T>
	T* GetSibling() { return (T*)GetSibling(T::StaticType()); }
	Component* GetSibling(const ComponentType& type);

	template<typename T>
	T* GetOrAddSibling() { return (T*)GetSibling(T::StaticType()); }
	Component* GetOrAddSibling(const ComponentType& type);

protected:
	template<class T>
	void RegisterProperty(const char* name, const void* prop);

	ComponentType m_Type;

private:
	Entity* const m_Entity;
	std::vector<Property> m_PropertyList;
};

template<class T>
void Component::RegisterProperty(const char* name, const void* value)
{
	m_PropertyList.push_back(Property(name, value, &propertyutils::PropertyToString<T>));
}

/**	Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<Component*>(const char* name, Component*& value);