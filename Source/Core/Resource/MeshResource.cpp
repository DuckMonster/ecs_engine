#include "CorePCH.h"
#include "MeshResource.h"

/**	Load
*******************************************************************************/
bool MeshResource::Load(const char* path)
{
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

	m_VAO = vao;
	m_DrawMode = GL_TRIANGLES;
	m_DrawCount = 3;
	m_Elements = false;

	return Resource::Load(path);
}

/**	Release
*******************************************************************************/
void MeshResource::Release()
{
	glDeleteVertexArrays(1, &m_VAO);
}

#include "ResourceManager.h"

/**	Serialize Specialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<MeshResource*>(const char* name, MeshResource*& value)
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->LoadMesh(resourceName);
	return true;
}