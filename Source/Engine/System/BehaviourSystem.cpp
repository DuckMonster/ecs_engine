#include "CorePCH.h"
#include "BehaviourSystem.h"
#include "Core/Script/ScriptFunction.h"
#include "Core/Tools/Time.h"

void BehaviourSystem::RunInternal(Entity* entity, BehaviourComponent* behaviour)
{
	if (behaviour->m_Script == nullptr)
		return;

	ScriptFunction function = behaviour->m_Script->GetFunction("void OnBehaviour(Entity@, float)");
	if (!function.IsValid())
		return;

	function.Call(entity, Time::FrameDelta());
}