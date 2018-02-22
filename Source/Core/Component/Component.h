#pragma once
#include "ComponentType.h"
#include "Core/Utils/StringUtils.h"

class Entity;
class IArchive;

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

#define VOIDP_TO_VALUE(type) type& value = *(type*)prop

	template<typename T>
	static std::string PropertyToString(const void* prop)
	{
		VOIDP_TO_VALUE(T);

		std::stringstream stream;
		stream << value;

		return stream.str();
	}

	template<>
	static std::string PropertyToString<glm::vec2>(const void* prop)
	{
		VOIDP_TO_VALUE(glm::vec2);
		return StringUtils::Printf("( %f, %f )", value.x, value.y);
	}

	template<>
	static std::string PropertyToString<glm::vec3>(const void* prop)
	{
		VOIDP_TO_VALUE(glm::vec3);
		return StringUtils::Printf("( %f, %f, %f )", value.x, value.y, value.z);
	}

	template<>
	static std::string PropertyToString<glm::vec4>(const void* prop)
	{
		VOIDP_TO_VALUE(glm::vec4);
		return StringUtils::Printf("( %f, %f, %f, %f )", value.x, value.y, value.z, value.w);
	}

	template<>
	static std::string PropertyToString<glm::quat>(const void* prop)
	{
		VOIDP_TO_VALUE(glm::quat);
		return StringUtils::Printf("( %f | %f, %f, %f )", value.w, value.x, value.y, value.z);
	}

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

protected:
	virtual void Serialize(IArchive* archive);

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
	m_PropertyList.push_back(Property(name, value, &Component::PropertyToString<T>));
}