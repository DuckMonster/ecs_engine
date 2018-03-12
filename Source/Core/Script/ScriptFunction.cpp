#include "CorePCH.h"

#include "ScriptFunction.h"
#include "ScriptEngine.h"
#include <angelscript.h>

/**	ScriptContext Pimpl Definition
*******************************************************************************/
struct ScriptFunction::ScriptContext
{
	asIScriptContext* asContext = nullptr;
	int argIndex = 0;
};

/**	Constructor
*******************************************************************************/
ScriptFunction::ScriptFunction(const char* module, const char* declaration)
{
	ScriptEngine* engine = ScriptEngine::GetInstance();

	asIScriptEngine* asEngine = engine->GetAngelScriptEngine();
	asIScriptModule* asModule = asEngine->GetModule(module);

	m_Function = asModule->GetFunctionByDecl(declaration);
}

/**	Destructor
*******************************************************************************/
ScriptFunction::~ScriptFunction()
{

}

/**	Prepare Context
*******************************************************************************/
void ScriptFunction::PrepareContext()
{
	if (!IsValid())
		return;

	ScriptEngine* engine = ScriptEngine::GetInstance();

	m_CurrentContext = new ScriptContext();
	m_CurrentContext->asContext = engine->GetAngelScriptEngine()->CreateContext();
	m_CurrentContext->asContext->Prepare(m_Function);
}

/**	Execute Context
*******************************************************************************/
void ScriptFunction::ExecuteContext()
{
	if (!Ensure(m_CurrentContext != nullptr))
		return;

	int executeResult = m_CurrentContext->asContext->Execute();
	Ensure(executeResult == asEXECUTION_FINISHED);
}

/**	Clean Context
*******************************************************************************/
void ScriptFunction::CleanContext()
{
	m_CurrentContext->asContext->Release();

	delete m_CurrentContext;
	m_CurrentContext = nullptr;
}

/**	Push Variable Specializations
*******************************************************************************/
#define SPEC_DEF(type, funcType) \
template<> \
void ScriptFunction::PushVar<type>(type& value) \
{ \
	if (!Ensure(m_CurrentContext != nullptr)) \
		return; \
	m_CurrentContext->asContext->SetArg ## funcType ## (m_CurrentContext->argIndex++, value); \
}

SPEC_DEF(int8, Byte)
SPEC_DEF(uint8, Byte)
SPEC_DEF(int16, Word)
SPEC_DEF(uint16, Word)
SPEC_DEF(int32, DWord)
SPEC_DEF(uint32, DWord)
SPEC_DEF(int64, QWord)
SPEC_DEF(uint64, QWord)
SPEC_DEF(float, Float)
SPEC_DEF(double, Double)
SPEC_DEF(void*, Object)