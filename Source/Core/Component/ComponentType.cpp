#include "CorePCH.h"
#include "ComponentType.h"

const ComponentType::id_t ComponentType::INVALID_ID = -1;

const ComponentType& ComponentType::FromId(id_t id)
{
	return GetIdTypeMap()[id];
}

const ComponentType& ComponentType::FromString(const char* name)
{
	return GetStringTypeMap()[name];
}