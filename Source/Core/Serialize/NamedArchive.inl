#include <rapidjson/rapidjson.h>
#include "Core/Tools/Math.h"

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
