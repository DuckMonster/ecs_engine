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

private:
	Source& m_Source;
	rapidjson::Pointer m_Pointer;
};

#include "NamedArchive.inl"