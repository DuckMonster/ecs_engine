#include "CorePCH.h"
#include "Program.h"

#include "../Component/ComponentType.h"
#include "../World/World.h"
#include "../Resource/ResourceManager.h"

/**	Constructor
*******************************************************************************/
Program::Program()
{
	// Init components
	ComponentType::InitializeTypes();

	// Map
	m_World = new World();
	m_World->LoadMap("Resource/Maps/testmap.json");
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

