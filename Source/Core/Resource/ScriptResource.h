#pragma once
#include "Resource.h"

class asIScriptModule;
class ScriptFunction;

class ScriptResource : public Resource
{
public:
	bool Load( const FFile& file ) override;
	void Release() override;

	ScriptFunction GetFunction( const char* declaration );

private:
	asIScriptModule* m_Module = nullptr;
};

#include "Core/Serialize/NamedArchive.h"

/**	Script Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<ScriptResource*>( const char* name, ScriptResource*& value );