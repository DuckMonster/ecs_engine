#include "CorePCH.h"
#include "ComponentType.h"

const ComponentType::id_t ComponentType::INVALID_ID = -1;
std::map<std::string, ComponentType> ComponentType::s_StringTypeMap;
std::unordered_map<ComponentType::id_t, ComponentType> ComponentType::s_IdTypeMap;

const ComponentType& ComponentType::FromId(id_t id)
{
	return s_IdTypeMap[id];
}

const ComponentType& ComponentType::FromString(const char* name)
{
	return s_StringTypeMap[name];
}
