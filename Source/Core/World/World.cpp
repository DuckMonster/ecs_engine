#include "CorePCH.h"
#include "World.h"
#include "Core/Utils/GLUtils.h"
#include "Core/Entity/Entity.h"
#include "Core/System/System.h"
#include "Core/Context/Context.h"
#include "Core/Serialize/NamedArchive.h"
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceManager.h"

// FOR TESTING, REMOVE LATER
#include "Engine/Engine.h"

using namespace glm;

Entity* CreateTestMesh(World* world, FName name)
{
	Entity* entity = world->CreateEntity(name);
	{
		RenderableComponent* renderComp = entity->AddComponent<RenderableComponent>();
		TransformComponent* transform = entity->AddComponent<TransformComponent>();

		float data[] {
			-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			0.f, 0.5f, 0.f
		};

		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		{
			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

			glBindVertexArray(0);
		}

		GLuint shader = GLUtils::CreateShaderFromFile("Resource/Shader/test");

		renderComp->m_ShaderProgram = shader;
	}

	return entity;
}

/**	Constructor
*******************************************************************************/
World::World()
{
	//Entity* entity = nullptr;

	//entity = CreateTestMesh("TestMesh1");
	//entity->GetComponent<TransformComponent>()->SetPosition(vec3(0.f, 0.f, 0.f));
	//entity->GetComponent<TransformComponent>()->SetRotation(angleAxis(pi<float>(), vec3(0.f, 0.f, 1.f)));
	//m_EntityList.push_back(entity);

	//entity = CreateTestMesh("TestMesh2");
	//entity->GetComponent<TransformComponent>()->SetPosition(vec3(2.f, 0.f, 0.f));
	//m_EntityList.push_back(entity);

	//entity = CreateTestMesh("TestMesh3");
	//entity->GetComponent<TransformComponent>()->SetPosition(vec3(1.f, 0.f, 2.f));
	//entity->GetComponent<TransformComponent>()->SetRotation(angleAxis(pi<float>() / 2.f, vec3(0.f, 1.f, 0.f)));
	//m_EntityList.push_back(entity);

	//entity = new Entity("Camera");
	//m_EntityList.push_back(entity);
	//{
	//	RenderSingletonComponent* renderSingleton = GetSingletonComponent<RenderSingletonComponent>();
	//	Context* context = Context::GetInstance();

	//	glm::mat4 proj, view;

	//	float width = context->width,
	//		height = context->height,
	//		ratio = width / height;

	//	proj = glm::ortho(-ratio * 5.f, ratio * 5.f, -5.f, 5.f, -10.f, 10.f);

	//	TransformComponent* transform = entity->AddComponent<TransformComponent>();
	//	transform->SetRotation(glm::angleAxis(glm::pi<float>() / 2.f, glm::vec3(0.f, 1.f, 0.f)));

	//	CameraComponent* camera = entity->AddComponent<CameraComponent>();
	//	camera->m_ProjectionMatrix = proj;

	//	renderSingleton->m_CurrentCamera = camera;
	//}

	m_SystemList.push_back(new CameraSystem(this));
	m_SystemList.push_back(new RenderSystem(this));

	PrintWorld();
}

/**	Destructor
*******************************************************************************/
World::~World()
{

}

/**	Do Frame
*******************************************************************************/
void World::DoFrame(float delta)
{
	//-------------------- Print frame rate
	static float FRAME_TIMER = 0.f;

	if ((FRAME_TIMER += delta) > 1.f)
	{
		FRAME_TIMER = 0.f;
		Debug_Log("Hello World, MS = %f", delta);
		PrintWorld();
	}

#if DEBUG
	static float UPDATE_TIMER = 0.f;

	if ((UPDATE_TIMER += delta) > 0.5f)
	{
		if (m_MapResource->HasChanged())
			LoadFromResource();

		UPDATE_TIMER = 0.f;
	}
#endif

	RunSystems();
}

/**	Create Entity
*******************************************************************************/
Entity* World::CreateEntity(FName& name)
{
	entity_id id = m_LastEntityId++;

	// Id overflow
	Ensure(id != MAX_ENTITY);

#if DEBUG
	Ensure(m_EntityLookup[id] == nullptr);
#endif

	Entity* newEntity = new Entity(id, name);
	m_EntityList.push_back(newEntity);
	m_EntityLookup[id] = newEntity;

	Debug_Log("Entity [%d] created: \"%s\"", id, name.c_str());

	return newEntity;
}

/**	Get Entity
*******************************************************************************/
Entity* World::GetEntity(entity_id id)
{
	return m_EntityLookup[id];
}

/**	Find Entity
*******************************************************************************/
Entity* World::FindEntity(const FName& name)
{
	for (Entity* entity : m_EntityList)
	{
		if (entity->GetName() == name)
			return entity;
	}

	return nullptr;
}

/**	Destroy Entity
*******************************************************************************/
void World::DestroyEntity(Entity* entity) { DestroyEntity(entity->GetId()); }
void World::DestroyEntity(entity_id id)
{
	Entity* entity = m_EntityLookup[id];

	if (!Ensure(entity))
		return;

	m_EntityLookup[id] = nullptr;

	std::vector<Entity*>::iterator it = std::find(m_EntityList.begin(), m_EntityList.end(), entity);
	if (Ensure(it != m_EntityList.end()))
		m_EntityList.erase(it);
}

/**	Load Map
*******************************************************************************/
void World::LoadMap(const char* path)
{
	if (!Ensure(m_MapResource == nullptr))
		return;

	m_MapResource = ResourceManager::GetInstance()->Load(path);
	LoadFromResource();
}

/**	Print World
*******************************************************************************/
void World::PrintWorld()
{
	Debug_Log("\n");
	Debug_Log("--- WORLD DUMP\n");

	for(Entity* entity : m_EntityList)
	{
		Debug_Log(entity->GetName().c_str());

		for (Component* comp : entity->GetAllComponents())
			comp->DebugPrint();
	}

	Debug_Log("--- WORLD END");
}

/**	Run Systems
*******************************************************************************/
void World::RunSystems()
{
	for (ISystem* system : m_SystemList)
	{
		system->Run();
	}
}

/**	Load Map
*******************************************************************************/
void World::LoadFromResource()
{
	if (m_MapResource == nullptr)
		return;

	NamedArchive::Source source = NamedArchive::Open(m_MapResource->GetPath());
	NamedArchive archive(source);

	const char* mapName = nullptr;
	archive.Serialize("name", mapName);

	if (!Ensure(mapName))
		return;

	Debug_Log("Loading map \"%s\"...", mapName);

	{
		//--------------------------------------------------- Read Entities
		NamedArchive entityListArchive = archive.Push("entities");
		Ensure(entityListArchive.IsArray());

		for (::uint32 i = 0; i < entityListArchive.ArraySize(); ++i)
		{
			// Get name and create
			NamedArchive entityArchive = entityListArchive.Push(i);
			const char* entityName = nullptr;
			if (!Ensure(entityArchive.Serialize("name", entityName)))
				continue;

			// Try to find and modify, otherwise it's a new entity!
			Entity* entity = FindEntity(FName(entityName));

			if (!entity)
				entity = CreateEntity(FName(entityName));

			// Read components
			NamedArchive componentListArchive = entityArchive.Push("components");

			Ensure(componentListArchive.IsArray());
			for (::uint32 i = 0; i < componentListArchive.ArraySize(); ++i)
			{
				NamedArchive componentArchive = componentListArchive.Push(i);

				// Get type
				const char* componentType = nullptr;
				if (!Ensure(componentArchive.Serialize("type", componentType)))
					continue;

				ComponentType type = ComponentType::FromString(componentType);
				if (!Ensure(type.IsValid()))
					continue;

				// Find or create this component
				Component* comp = entity->GetComponent(type);

				if (!comp)
					comp = entity->AddComponent(type);

				comp->Serialize(componentArchive);
			}
		}
	}

	PrintWorld();
}
