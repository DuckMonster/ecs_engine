#pragma once
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

class NamedArchive
{
public:
	struct Source
	{
		rapidjson::Document m_Document;
	};
	static Source Open(const char* path);

public:
	NamedArchive(Source& source) : m_Source(source), m_Pointer() {}
private:
	NamedArchive(Source& source, rapidjson::Pointer pointer) :
		m_Source(source), m_Pointer(pointer) {}

public:
	template<typename T>
	bool Serialize(const char* name, T& value);

	template<typename T>
	bool SerializeArray(const char* name, T* arrayPtr, uint32 count);

	template<typename T>
	bool SerializeArray(const char* name, std::vector<T>& value);

	NamedArchive Push(const char* name);
	NamedArchive Push(uint32 index);

	bool IsArray();
	uint32 ArraySize();
	bool IsValid();

private:
	Source& m_Source;
	rapidjson::Pointer m_Pointer;
};

// Some default serialize specializations
//--------------------------------------------------- 
template<>
bool NamedArchive::Serialize<std::string>(const char* name, std::string& value);
template<>
bool NamedArchive::Serialize<glm::vec2>(const char* name, glm::vec2& value);
template<>
bool NamedArchive::Serialize<glm::vec3>(const char* name, glm::vec3& value);
template<>
bool NamedArchive::Serialize<glm::vec4>(const char* name, glm::vec4& value);
template<>
bool NamedArchive::Serialize<glm::quat>(const char* name, glm::quat& value);
//--------------------------------------------------- 

#include "NamedArchive.inl"