#pragma once
#include "Core/Tools/SingletonClass.h"

class asIScriptEngine;

class ScriptEngine : public SingletonClass<ScriptEngine>
{
public:
	static void RegisterFunction(const char* signature, void* fptr);

public:
	ScriptEngine();
	~ScriptEngine();

	asIScriptEngine* GetAngelScriptEngine() const { return m_Engine; };

private:
	void RegisterFunction_Internal(const char* signature, void* fptr);

public:
	asIScriptEngine* m_Engine = nullptr;
};