#include "CorePCH.h"
#include "BehaviourSystem.h"
#include "Core/Script/ScriptFunction.h"
#include "Core/Tools/Time.h"

void BehaviourSystem::RunInternal(Entity* entity, BehaviourComponent* behaviour)
{
	behaviour->OnUpdate();
}