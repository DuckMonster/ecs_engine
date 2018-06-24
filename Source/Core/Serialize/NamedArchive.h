#pragma once
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include "Archive.h"

#define DEFAULT_TYPE(type) bool Serialize(const char* name, type& value) override { SerializeDefault<type>(name, value); }
#define DEFINE_TYPE(type) bool Serialize(const char* name, type& value) override

class NamedArchive : public IArchive
{
public:
	struct Source
	{
		rapidjson::Document document;
	};
	static Source Open(const char* path);

public:
	NamedArchive(Source& source) : source(source), pointer() {}
private:
	NamedArchive(Source& source, rapidjson::Pointer pointer) :
		source(source), pointer(pointer) {}

public:
	// Default types 
	DEFAULT_TYPE(int8);
	DEFAULT_TYPE(uint8);
	DEFAULT_TYPE(int16);
	DEFAULT_TYPE(uint16);
	DEFAULT_TYPE(int32);
	DEFAULT_TYPE(uint32);
	DEFAULT_TYPE(float);
	DEFAULT_TYPE(double);
	DEFAULT_TYPE(char*);

	// Special types
	DEFINE_TYPE(glm::vec2);
	DEFINE_TYPE(glm::vec3);
	DEFINE_TYPE(glm::vec4);
	DEFINE_TYPE(glm::quat);
	DEFINE_TYPE(std::string);

	NamedArchive Push(const char* name);
	NamedArchive Push(uint32 index);

	template<typename T>
	bool SerializeArray(const char* name, std::vector<T>& value);

	bool IsArray();
	uint32 ArraySize();
	bool IsValid();

private:
	// Default serialization that rapidjson can handle
	template<typename T>
	bool SerializeDefault(const char* name, T& value)
	{
		rapidjson::Value* jsonValue = pointer.Get(source.document);
		if (!jsonValue)
			return false;

		if (!Ensure(jsonValue->Is<T>()))
			return false;

		value = jsonValue->Get<T>();
		return false;
	}

	Source& source;
	rapidjson::Pointer pointer;
};

#undef DEFAULT_TYPE
#undef DEFINE_TYPE

#include "NamedArchive.inl"