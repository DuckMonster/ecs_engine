#include "CorePCH.h"
#include "ScriptEngine.h"
#include "Core/Utils/File.h"
#include <angelscript.h>
#include "AngelScript/add_on/scriptstdstring/scriptstdstring.h"
#include "AngelScript/add_on/scriptmath/scriptmath.h"
#include "ScriptLibrary.h"

namespace 
{
	void MessageCallback(const asSMessageInfo* msg, void* param)
	{
		const char* type = "UNKNOWN";

		switch (msg->type)
		{
		case asMSGTYPE_INFORMATION: type = "Info"; break;
		case asMSGTYPE_WARNING: type = "Warning"; break;
		case asMSGTYPE_ERROR: type = "Error"; break;
		}

		std::string fileName = File::GetFileFromPath(msg->section);

		Debug_Log_Minimal("(ANGELSCRIPT) %s: %s(%d, %d)> %s", type, fileName.c_str(), msg->row, msg->col, msg->message);
	}
}

/**	Register Function
*******************************************************************************/
void ScriptEngine::RegisterFunction(const char* signature, void* fptr)
{

}

/**	Constructor
*******************************************************************************/
ScriptEngine::ScriptEngine()
{
	m_Engine = asCreateScriptEngine();
	m_Engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
	RegisterStdString(m_Engine);
	RegisterScriptMath(m_Engine);
	RegisterDefaultScriptLibrary(m_Engine);
}

/**	Destructor
*******************************************************************************/
ScriptEngine::~ScriptEngine()
{
	m_Engine->ShutDownAndRelease();
	m_Engine = nullptr;
}

/**	Register Function (Internal)
*******************************************************************************/
void ScriptEngine::RegisterFunction_Internal(const char* signature, void* fptr)
{
}
