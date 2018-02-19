#include "CorePCH.h"
#include "World.h"
#include "Core/Utils/GLUtils.h"
#include "Core/Entity/Entity.h"
#include "Core/System/System.h"
#include "Core/Context/Context.h"
#include "Engine/Engine.h"

using namespace glm;

Entity* CreateTestMesh(FName name)
{
	Entity* entity = new Entity(name);
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
		renderComp->m_VertexObject = vao;
		renderComp->m_DrawCount = 3;
		renderComp->m_DrawMode = GL_TRIANGLES;
		renderComp->m_UsingElements = false;
	}

	return entity;
}

/**	Constructor
*******************************************************************************/
World::World()
{
	Entity* entity = nullptr;

	entity = CreateTestMesh("TestMesh1");
	entity->GetComponent<TransformComponent>()->SetPosition(vec3(0.f, 0.f, 0.f));
	entity->GetComponent<TransformComponent>()->SetRotation(angleAxis(pi<float>(), vec3(0.f, 0.f, 1.f)));
	m_EntityList.push_back(entity);

	entity = CreateTestMesh("TestMesh2");
	entity->GetComponent<TransformComponent>()->SetPosition(vec3(2.f, 0.f, 0.f));
	m_EntityList.push_back(entity);

	entity = CreateTestMesh("TestMesh3");
	entity->GetComponent<TransformComponent>()->SetPosition(vec3(1.f, 0.f, 2.f));
	entity->GetComponent<TransformComponent>()->SetRotation(angleAxis(pi<float>() / 2.f, vec3(0.f, 1.f, 0.f)));
	m_EntityList.push_back(entity);

	entity = new Entity("Camera");
	m_EntityList.push_back(entity);
	{
		RenderSingletonComponent* renderSingleton = GetSingletonComponent<RenderSingletonComponent>();
		Context* context = Context::GetInstance();

		glm::mat4 proj, view;

		float width = context->width,
			height = context->height,
			ratio = width / height;

		proj = glm::ortho(-ratio * 5.f, ratio * 5.f, -5.f, 5.f, -10.f, 10.f);

		TransformComponent* transform = entity->AddComponent<TransformComponent>();
		transform->SetRotation(glm::angleAxis(glm::pi<float>() / 2.f, glm::vec3(0.f, 1.f, 0.f)));

		CameraComponent* camera = entity->AddComponent<CameraComponent>();
		camera->m_ProjectionMatrix = proj;

		renderSingleton->m_CurrentCamera = camera;
	}

	m_SystemList.push_back(new CameraSystem(this));
	m_SystemList.push_back(new RenderSystem(this));
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
		printf("Hello World, MS = %f\n", delta);

		PrintWorld();
	}

	RunSystems();
}

/**	Print World
*******************************************************************************/
void World::PrintWorld()
{
	std::stringstream builder;

	builder << "\n";
	builder << "--- WORLD DUMP\n";

	for(Entity* entity : m_EntityList)
	{
		builder << entity->GetName().c_str() << "\n";
	}

	Debug_Log(builder.str().c_str());
}

void World::RunSystems()
{
	for (ISystem* system : m_SystemList)
	{
		system->Run();
	}
}
