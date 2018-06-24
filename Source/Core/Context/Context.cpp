#include "CorePCH.h"
#include "Context.h"
#include "ContextGLFW.h"

Context* Context::CurrentContext = nullptr;

/**	Create Context
*******************************************************************************/
Context* Context::Create()
{
	return new ContextGLFW();
}

/**	Get Current
*******************************************************************************/
Context* Context::Get()
{
	return CurrentContext;
}

/**	Make Current
*******************************************************************************/
void Context::MakeCurrent()
{
	CurrentContext = this;
}