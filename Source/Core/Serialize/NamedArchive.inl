#include <rapidjson/rapidjson.h>
#include "Core/Tools/Math.h"

template<typename T>
bool NamedArchive::Serialize(const char* name, T& value)
{
	using namespace rapidjson;
	Pointer ptr = pointer.Append(name);

	Value* valuePtr = ptr.Get(source.document);
	if (!valuePtr)
		return false;

	Value& valueRef = *valuePtr;

	if (!Ensure(valueRef.Is<T>()))
		return false;

	value = valueRef.Get<T>();
	return true;
}

/**	Serialize Array
*******************************************************************************/
template<typename T>
bool NamedArchive::SerializeArray(const char* name, T* arrayPtr, uint32 count)
{
	rapidjson::Pointer ptr = pointer.Append(name);
	rapidjson::Value* valuePtr = pointer.Append(name).Get(source.document);

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

/**	Serialize Array
*******************************************************************************/
template<typename T>
bool NamedArchive::SerializeArray( const char* name, std::vector<T>& value )
{
	rapidjson::Pointer ptr = pointer.Append(name);
	rapidjson::Value* valuePtr = pointer.Append(name).Get(source.document);

	if (!valuePtr)
		return false;

	rapidjson::Value& valueRef = *valuePtr;

	if (!valueRef.IsArray())
		return false;

	value.reserve(valueRef.Size());

	for (::uint32 i = 0; i < valueRef.Size(); ++i)
	{
		if (!Ensure(valueRef[i].Is<T>()))
			continue;

		value.push_back(valueRef[i].Get<T>());;
	}

	return true;
}
