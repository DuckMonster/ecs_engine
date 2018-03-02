#pragma once
#include "Resource.h"

class asIScriptModule;
class ScriptFunction;

class ScriptResource : public Resource
{
public:
	ScriptResource(ResourceManager* manager, guid_t hash) : Resource(manager, hash) {}

	bool Load(const char* path) override;
	void Release() override;

	ScriptFunction GetFunction(const char* declaration);

private:
	asIScriptModule* m_Module;
};

#include "Core/Serialize/NamedArchive.h"

/**	Script Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<ScriptResource*>(const char* name, ScriptResource*& value);