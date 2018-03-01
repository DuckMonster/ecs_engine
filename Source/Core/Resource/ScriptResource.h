#pragma once
#include "Resource.h"

class asIScriptModule;

class ScriptResource : public Resource
{
public:
	ScriptResource(ResourceManager* manager, guid_t hash) : Resource(manager, hash) {}

	bool Load(const char* path) override;
	void Release() override;

private:
	asIScriptModule* m_Module;
	std::string m_ModuleName;
};

#include "Core/Serialize/NamedArchive.h"

/**	Script Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<ScriptResource*>(const char* name, ScriptResource*& value);