#include "CorePCH.h"
#include "Component.h"
#include "Core/Utils/StringUtils.h"

namespace
{
#define VOIDP_TO_VALUE(type) type& value = *(type*)prop

	template<typename T>
	std::string PropertyToString(void* prop)
	{
		VOIDP_TO_VALUE(T);

		std::stringstream stream;
		stream << value;

		return stream.str();
	}

	template<>
	std::string PropertyToString<glm::vec2>(void* prop)
	{
		VOIDP_TO_VALUE(glm::vec2);
		return StringUtils::Printf("( %f, %f )", value.x, value.y);
	}

	template<>
	std::string PropertyToString<glm::vec3>(void* prop)
	{
		VOIDP_TO_VALUE(glm::vec3);
		return StringUtils::Printf("( %f, %f, %f )", value.x, value.y, value.z);
	}

	template<>
	std::string PropertyToString<glm::vec4>(void* prop)
	{
		VOIDP_TO_VALUE(glm::vec4);
		return StringUtils::Printf("( %f, %f, %f, %f )", value.x, value.y, value.z, value.w);
	}

	template<>
	std::string PropertyToString<glm::quat>(void* prop)
	{
		VOIDP_TO_VALUE(glm::quat);
		return StringUtils::Printf("( %f | %f, %f, %f )", value.w, value.x, value.y, value.z);
	}
}

void Component::DebugPrint()
{
	Debug_Log("\t[ %s ]", m_Type.Name());
	for (Property& prop : m_PropertyList)
	{
		Debug_Log("\t\t%s = %s", prop.m_Name, prop.m_StringFunc(prop.m_Value).c_str());
	}

	Debug_Log("");
}

void Component::Initialize()
{
}

void Component::Serialize(NamedArchive& archive)
{
}
