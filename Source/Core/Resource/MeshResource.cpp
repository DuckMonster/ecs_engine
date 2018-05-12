#include "CorePCH.h"
#include "MeshResource.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

/**	Load
*******************************************************************************/
bool MeshResource::Load(const char* path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene)
	{
		Debug_Log("Failed to load mesh \"%s\": %s", path, importer.GetErrorString());
		return false;
	}

	uint32 numVerts = 0;
	std::vector<glm::vec3> positionData;
	std::vector<glm::vec3> normalData;
	std::vector<glm::vec2> uvData;

	for(uint32 meshIndex=0; meshIndex<scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];

		GLuint meshVerts = mesh->mNumVertices;
		positionData.resize(numVerts + meshVerts);
		normalData.resize(numVerts + meshVerts);
		uvData.resize(numVerts + meshVerts);

		memcpy(&positionData[numVerts], mesh->mVertices, sizeof(glm::vec3) * meshVerts);
		
		if (mesh->HasNormals())
			memcpy(&normalData[numVerts], mesh->mNormals, sizeof(glm::vec3) * meshVerts);

		if (mesh->HasTextureCoords(0))
			memcpy(&uvData[numVerts], mesh->mTextureCoords[0], sizeof(glm::vec2) * meshVerts);

		numVerts += meshVerts;
	}

	glGenVertexArrays(1, &m_Data.VertexObject);
	glGenBuffers(3, m_Buffers);

	{
		glBindVertexArray(m_Data.VertexObject);

		// Upload position
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numVerts, &positionData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

		// Upload normals
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numVerts, &normalData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

		// Upload UV
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * numVerts, &uvData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

		glBindVertexArray(0);
	}

	m_Data.DrawMode = GL_TRIANGLES;
	m_Data.DrawCount = positionData.size();
	m_Data.UseElements = false;

	return Resource::Load(path);
}

/**	Release
*******************************************************************************/
void MeshResource::Release()
{
	glDeleteVertexArrays(1, &m_Data.VertexObject);
	glDeleteBuffers(3, m_Buffers);

	Resource::Release();
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