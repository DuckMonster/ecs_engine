#include "CorePCH.h"
#include "Program.h"
#include "Core/Context/Context.h"

#include "../Component/ComponentType.h"
#include "../World/World.h"
#include "../Resource/ResourceManager.h"
#include "../Rendering/Pipeline/Deferred.h"

/**	Constructor
*******************************************************************************/
Program::Program()
{
	// Init components
	ComponentType::InitializeTypes();

	// Init render pipeline
	Rendering::IRenderPipeline::Create<Rendering::DeferredPipeline>();

	// Map
	m_World = new World();
	m_World->LoadMap("Resource/Maps/uvmap.json");
}

/**	Destructor
*******************************************************************************/
Program::~Program()
{
	delete m_World;
}

/**	Do Frame
*******************************************************************************/
void Program::DoFrame()
{
	// Update resources
	ResourceManager::GetInstance()->UpdateResourceHotReloading();

	// Update delta
	Time::UpdateDelta();

	// Run world
	m_World->DoFrame();
}

