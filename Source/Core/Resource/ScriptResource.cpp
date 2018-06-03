#include "CorePCH.h"
#include "ScriptResource.h"
#include <angelscript.h>
#include "AngelScript/add_on/scriptbuilder/scriptbuilder.h"
#include "Core/Script/ScriptEngine.h"
#include "Core/Script/ScriptFunction.h"
#include "Core/OS/File.h"

namespace
{
	std::string GetModuleNameFromPath(const char* path)
	{
		int start = 0;
		size_t end = -1;

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
bool ScriptResource::Load(const FFile& file)
{
	std::string moduleName = GetModuleNameFromPath(file.GetPath());

	CScriptBuilder builder;
	asIScriptEngine* engine = ScriptEngine::GetInstance()->GetAngelScriptEngine();

	if (!Ensure(builder.StartNewModule(engine, moduleName.c_str()) >= 0))
		return false;

	if (!Ensure(builder.AddSectionFromFile(file.GetPath()) >= 0))
		return false;

	// It's OK if the module doesn't build properly, just move on
	// We want to keep the resource around anyways for hot-reloading
	if (builder.BuildModule() <= 0)
		m_Module = engine->GetModule(moduleName.c_str());

	return true;
}

/**	Release
*******************************************************************************/
void ScriptResource::Release()
{
	if (m_Module == nullptr)
		return;

	asIScriptEngine* engine = ScriptEngine::GetInstance()->GetAngelScriptEngine();
	engine->DiscardModule(m_Module->GetName());

	m_Module = nullptr;
	Resource::Release();
}

/**	Get Function
*******************************************************************************/
ScriptFunction ScriptResource::GetFunction(const char* declaration)
{
	// Not having a module is fine, it just means the script didn't compile
	if (!m_Module)
		return ScriptFunction();

	return ScriptFunction(m_Module->GetName(), declaration);
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

	value = ResourceManager::GetInstance()->Load<ScriptResource>(resourceName);
	return true;
}
