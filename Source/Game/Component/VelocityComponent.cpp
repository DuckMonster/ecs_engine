#include "CorePCH.h"
#include "VelocityComponent.h"
#include "Engine/Component/BehaviourComponent.h"

using namespace glm;

/**	Start
*******************************************************************************/
void VelocityComponent::Start()
{
	BIND_BEHAVIOUR(&VelocityComponent::UpdateVelocity);
}

/**	Update Velocity
*******************************************************************************/
void VelocityComponent::UpdateVelocity()
{

}
