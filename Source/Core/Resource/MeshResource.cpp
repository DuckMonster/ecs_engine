#include "CorePCH.h"
#include "MeshResource.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

/**	Load
*******************************************************************************/
bool MeshResource::Load(const char* path)
{
	float data[] {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.f, 0.5f, 0.f
	};

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene)
	{
		Debug_Log("Failed to load mesh \"%s\": %s", path, importer.GetErrorString());
		return false;
	}

	std::vector<glm::vec3> positionData;

	for(uint32 meshIndex=0; meshIndex<scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		positionData.resize(mesh->mNumVertices);
		memcpy(&positionData[0], mesh->mVertices, sizeof(glm::vec3) * mesh->mNumVertices);
	}

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positionData.size(), &positionData[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

		glBindVertexArray(0);
	}

	m_VAO = vao;
	m_DrawMode = GL_TRIANGLES;
	m_DrawCount = positionData.size();
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