#pragma once

class asIScriptFunction;

class ScriptFunction
{
	struct ScriptContext;

public:
	ScriptFunction() {}
	ScriptFunction(const char* module, const char* declaration);
	~ScriptFunction();

	template<typename... TArgs>
	void Call(TArgs... args);

	bool IsValid() const { return m_Function != nullptr; }

private:
	void PrepareContext();
	void ExecuteContext();

	template<typename T>
	void PushVar(T& val);
	template<typename... TArgs>
	void PushVars(TArgs&...);
	template<typename T, typename... TRest>
	void PushVars(T& arg, TRest&... rest);

	void CleanContext();

private:
	asIScriptFunction* m_Function = nullptr;
	ScriptContext* m_CurrentContext = nullptr;
};

#define VAR_SPEC(type)\
template<>\
void ScriptFunction::PushVar<type>(type& value)

VAR_SPEC(int8);
VAR_SPEC(int16);
VAR_SPEC(int32);
VAR_SPEC(int64);
VAR_SPEC(uint8);
VAR_SPEC(uint16);
VAR_SPEC(uint32);
VAR_SPEC(uint64);
VAR_SPEC(float);
VAR_SPEC(double);
VAR_SPEC(void*);

#undef VAR_SPEC

/**	Call
*******************************************************************************/
template<typename... TArgs>
void ScriptFunction::Call(TArgs... args)
{
	// Invalid script is fine, just return
	if (!IsValid())
		return;

	PrepareContext();
	PushVars(args...);
	ExecuteContext();
	CleanContext();
}

/**	Push Variable Template
*******************************************************************************/
template<typename T>
void ScriptFunction::PushVar(T& val)
{
	static_assert(std::is_pointer<typename std::remove_reference<T>::type>::value, "Trying to call script function with an unknown non-pointer object");
	PushVar((void*&)val);
}

/**	Push Vars Base Case
*******************************************************************************/
template<typename... TArgs>
void ScriptFunction::PushVars(TArgs&...) { }

/**	Push Vars
*******************************************************************************/
template<typename T, typename... TRest>
void ScriptFunction::PushVars(T& arg, TRest&... rest)
{
	PushVar(arg);
	PushVars(rest...);
}