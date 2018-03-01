#pragma once
#include "Core/Utils/SingletonClass.h"

class asIScriptEngine;

class ScriptEngine : public SingletonClass<ScriptEngine>
{
public:
	ScriptEngine();
	~ScriptEngine();

	asIScriptEngine* GetAngelScriptEngine() const { return m_Engine; };

public:
	asIScriptEngine* m_Engine = nullptr;
};