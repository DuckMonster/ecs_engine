#include "CorePCH.h"
#include "ScriptLibrary.h"
#include <angelscript.h>

namespace
{
	/**	Namespace Scope Struct
	*******************************************************************************/
	struct NamespaceScope
	{
		NamespaceScope(asIScriptEngine* engine, const char* name) :
			m_Engine(engine)
		{
			m_Engine->SetDefaultNamespace(name);
		}
		~NamespaceScope()
		{
			m_Engine->SetDefaultNamespace("");
		}

	private:
		asIScriptEngine* m_Engine;
	};
}

//--------------------------------------------------- Debug Functions
namespace
{
	/**	Print
	*******************************************************************************/
	void Print(std::string message)
	{
		//Debug_Log_Minimal(message.c_str());
		printf("%s\n", message.c_str());
	}
}

//--------------------------------------------------- Time
#include "Core/Utils/Time.h"
namespace Time
{
	/**	Time::Elapsed
	*******************************************************************************/
	float Elapsed()
	{
		return FTime::TotalElapsed();
	}
}

//--------------------------------------------------- Entity
#include "Core/Entity/Entity.h"
namespace EntityFuncs
{
	/**	Get Name
	*******************************************************************************/
	std::string GetName(Entity* entity)
	{
		return entity->GetName().c_str();
	}
}

#define REGISTER_FUNC(decl, funcptr) do { int r = engine->RegisterGlobalFunction(decl, asFUNCTION(funcptr), asCALL_CDECL); Ensure(r >= 0); } while(0)

/**	Register Default Script Library
*******************************************************************************/
void RegisterDefaultScriptLibrary(class asIScriptEngine* engine)
{
	REGISTER_FUNC("void Print(string message)", Print);

	//--------------------------------------------------- Time
	{
		NamespaceScope scope(engine, "Time");
		REGISTER_FUNC("float Elapsed()", Time::Elapsed);
	}

	//--------------------------------------------------- Entity
	{
		engine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_REF | asOBJ_NOCOUNT);
		engine->RegisterObjectMethod("Entity", "string GetName()", asFUNCTION(EntityFuncs::GetName), asCALL_CDECL_OBJLAST);
	}
}