#include "CorePCH.h"
#include "NamedArchive.h"
#include "Core/Utils/File.h"
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
		printf("Failed to parse JSON file \"%s\": %s (%d)\n", path, GetParseError_En(result.Code()), result.Offset());

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
