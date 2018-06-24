#include "CorePCH.h"
#include "NamedArchive.h"
#include "Core/OS/File.h"
#include <rapidjson/error/en.h>

using namespace rapidjson;


/**	Open
*******************************************************************************/
NamedArchive::Source NamedArchive::Open(const char* path)
{
	Source source;

	std::string jsonString;
	FFile sourceFile(path);

	if (!sourceFile.ReadAll(jsonString))
	{
		Debug_Log("Failed to open json file \"%s\"", path);
		return source;
	}

	ParseResult result = source.document.Parse(jsonString.c_str());
	if (!result)
		Debug_Log("Failed to parse JSON file \"%s\": %s (%d)\n", path, GetParseError_En(result.Code()), result.Offset());

	return source;
}

/**	Push Name
*******************************************************************************/
NamedArchive NamedArchive::Push(const char* name)
{
	return NamedArchive(source, pointer.Append(name));
}

/**	Push Index
*******************************************************************************/
NamedArchive NamedArchive::Push(uint32 index)
{
	return NamedArchive(source, pointer.Append(index));
}

/**	Is Array
*******************************************************************************/
bool NamedArchive::IsArray()
{
	Value* value = pointer.Get(source.document);
	if (value == nullptr)
		return false;

	return value->IsArray();
}

/**	Array Size
*******************************************************************************/
uint32 NamedArchive::ArraySize()
{
	Value* value = pointer.Get(source.document);
	if (value == nullptr)
		return 0;

	return value->Size();
}

bool NamedArchive::IsValid()
{
	return pointer.Get(source.document) != nullptr;
}

#define IMPL_TYPE(type) bool NamedArchive::Serialize(const char* name, type& value)
IMPL_TYPE(glm::vec2)
{
	return SerializeArray<float>()
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