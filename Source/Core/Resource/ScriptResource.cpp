#include "CorePCH.h"
#include "ScriptResource.h"
#include <angelscript.h>
#include "AngelScript/add_on/scriptbuilder/scriptbuilder.h"
#include "Core/Script/ScriptEngine.h"
#include "Core/Utils/File.h"

namespace
{
	std::string GetModuleNameFromPath(const char* path)
	{
		int start = 0;
		int end = -1;

		for (int i = 0; i < strlen(path); ++i)
		{
			if (path[i] == '/' || path[i] == '\\')
				start = i + 1;

			if (path[i] == '.')
			{
				end = i;
				break;
			}
		}

		if (end == 0)
			end = strlen(path);

		std::string result(path + start, path + end);
		return result;
	}
}

/**	Load
*******************************************************************************/
bool ScriptResource::Load(const char* path)
{
	m_ModuleName = GetModuleNameFromPath(path);

	CScriptBuilder builder;
	asIScriptEngine* engine = ScriptEngine::GetInstance()->GetAngelScriptEngine();

	if (!Ensure(builder.StartNewModule(engine, m_ModuleName.c_str()) >= 0))
		return false;

	if (!Ensure(builder.AddSectionFromFile(path) >= 0))
		return false;

	if (!Ensure(builder.BuildModule() >= 0))
		return false;

	m_Module = engine->GetModule(m_ModuleName.c_str());
	return Resource::Load(path);
}

/**	Release
*******************************************************************************/
void ScriptResource::Release()
{
	asIScriptEngine* engine = ScriptEngine::GetInstance()->GetAngelScriptEngine();
	engine->DiscardModule(m_ModuleName.c_str());
}

#include "ResourceManager.h"

/**	Serialize Specialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<ScriptResource*>(const char* name, ScriptResource*& value)
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->LoadScript(resourceName);
	return true;
}
