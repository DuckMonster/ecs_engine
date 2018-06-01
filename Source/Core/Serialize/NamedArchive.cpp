#include "CorePCH.h"
#include "NamedArchive.h"
#include "Core/Tools/File.h"
#include <rapidjson/error/en.h>

using namespace rapidjson;

NamedArchive::Source NamedArchive::Open(const char* path)
{
	Source source;

	std::string jsonString;
	if (!Ensure(File::ReadFile(path, jsonString)))
	{
		Debug_Log("Failed to open json file \"%s\"", path);
		return source;
	}

	ParseResult result = source.m_Document.Parse(jsonString.c_str());
	if (!result)
		Debug_Log("Failed to parse JSON file \"%s\": %s (%d)\n", path, GetParseError_En(result.Code()), result.Offset());

	return source;
}

NamedArchive NamedArchive::Push(const char* name)
{
	return NamedArchive(m_Source, m_Pointer.Append(name));
}

NamedArchive NamedArchive::Push(uint32 index)
{
	return NamedArchive(m_Source, m_Pointer.Append(index));
}

bool NamedArchive::IsArray()
{
	Value* value = m_Pointer.Get(m_Source.m_Document);
	if (value == nullptr)
		return false;

	return value->IsArray();
}

uint32 NamedArchive::ArraySize()
{
	Value* value = m_Pointer.Get(m_Source.m_Document);
	if (value == nullptr)
		return false;

	return value->Size();
}

//--------------------------------------------------- Specializations!
template<>
bool NamedArchive::Serialize<std::string>(const char* name, std::string& value)
{
	const char* charValue = nullptr;
	if (!Serialize(name, charValue))
		return false;

	value = charValue;
	return true;
}

template<>
bool NamedArchive::Serialize<glm::vec2>(const char* name, glm::vec2& value)
{
	return SerializeArray<float>(name, (float*)&value, 2);
}

template<>
bool NamedArchive::Serialize<glm::vec3>(const char* name, glm::vec3& value)
{
	return SerializeArray<float>(name, (float*)&value, 3);
}

template<>
bool NamedArchive::Serialize<glm::vec4>(const char* name, glm::vec4& value)
{
	return SerializeArray<float>(name, (float*)&value, 4);
}

template<>
bool NamedArchive::Serialize<glm::quat>(const char* name, glm::quat& value)
{
	// We don't want to write quaternions in text, so we do euler-angles instead
	glm::vec3 eulerAngles;
	if (!SerializeArray(name, (float*)&eulerAngles, 3))
		return false;

	value = Math::EulerToQuat(eulerAngles);
	return true;
}