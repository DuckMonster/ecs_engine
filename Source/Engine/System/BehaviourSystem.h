#include "Core/System/System.h"
#include "Engine/Component/BehaviourComponent.h"

class BehaviourSystem : public System<BehaviourComponent>
{
public:
	BehaviourSystem(World* world) : SystemBase(world) {}

protected:
	void RunInternal(Entity* entity, BehaviourComponent* behaviour) override;
};