#include <rapidjson/rapidjson.h>
#include "Core/Utils/Math.h"

template<typename T>
bool NamedArchive::Serialize(const char* name, T& value)
{
	using namespace rapidjson;
	Pointer ptr = m_Pointer.Append(name);

	Value* valuePtr = ptr.Get(m_Source.m_Document);
	if (!valuePtr)
		return false;

	Value& valueRef = *valuePtr;

	if (!Ensure(valueRef.Is<T>()))
		return false;

	value = valueRef.Get<T>();
	return true;
}

template<>
inline bool NamedArchive::Serialize<std::string>(const char* name, std::string& value)
{
	const char* charValue = nullptr;
	if (!Serialize(name, charValue))
		return false;

	value = charValue;
	return true;
}

template<>
inline bool NamedArchive::Serialize<glm::vec2>(const char* name, glm::vec2& value)
{
	return SerializeArray<float>(name, (float*)&value, 2);
}

template<>
inline bool NamedArchive::Serialize<glm::vec3>(const char* name, glm::vec3& value)
{
	return SerializeArray<float>(name, (float*)&value, 3);
}

template<>
inline bool NamedArchive::Serialize<glm::vec4>(const char* name, glm::vec4& value)
{
	return SerializeArray<float>(name, (float*)&value, 4);
}

template<>
inline bool NamedArchive::Serialize<glm::quat>(const char* name, glm::quat& value)
{
	// We don't want to write quaternions in text, so we do euler-angles instead
	glm::vec3 eulerAngles;
	if (!SerializeArray(name, (float*)&eulerAngles, 3))
		return false;

	value = Math::EulerToQuat(eulerAngles);
	return true;
}

template<typename T>
bool NamedArchive::SerializeArray(const char* name, T* arrayPtr, uint32 count)
{
	rapidjson::Pointer ptr = m_Pointer.Append(name);
	rapidjson::Value* valuePtr = m_Pointer.Append(name).Get(m_Source.m_Document);

	if (!valuePtr)
		return false;

	rapidjson::Value& valueRef = *valuePtr;

	Ensure(valueRef.IsArray());
	Ensure(valueRef.Size() == count);

	for (::uint32 i = 0; i < count; ++i)
	{
		if (!Ensure(valueRef[i].Is<T>()))
			continue;

		arrayPtr[i] = valueRef[i].Get<T>();
	}

	return true;
}