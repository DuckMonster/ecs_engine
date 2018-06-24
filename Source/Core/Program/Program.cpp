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
	// Init render pipeline
	Rendering::IRenderPipeline::Create<Rendering::DeferredPipeline>();

	// Map
	m_World = new World();
	m_World->LoadMap("Resource/Maps/testmap.json");

	// Escape is quit
	Context::Get()->OnKeyAction.BindLambda([](Key key, KeyAction action)
	{
		if (key == Key::Escape && action == KeyAction::Pressed)
			Context::Get()->Close();
	});
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