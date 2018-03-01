#pragma once
#include "ComponentType.h"
#include "Core/Utils/StringUtils.h"
#include "PropertyUtils.h"

class Entity;
class NamedArchive;

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

	virtual void Initialize();

	const ComponentType& Type() const { return m_Type; }
	static const ComponentType& StaticType()
	{
		static ComponentType INVALID_TYPE;
		return INVALID_TYPE;
	}

	void DebugPrint();
	virtual void Serialize(NamedArchive& archive);

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