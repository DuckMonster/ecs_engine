#include "CorePCH.h"
#include "MeshResource.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

/**	Load
*******************************************************************************/
bool MeshResource::Load(const FFile& file)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file.GetPath(), aiProcess_Triangulate);

	if (!scene)
	{
		Debug_Log("Failed to load mesh \"%s\": %s", file.GetPath(), importer.GetErrorString());
		return false;
	}

	m_Data.reserve(scene->mNumMeshes);

	/* **** LOAD MESHES **** */
	for(uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];

		uint32 meshVerts = mesh->mNumVertices;
		uint32 meshIndices = mesh->mNumFaces * 3;

		glm::vec3* positionData = new glm::vec3[meshVerts];
		glm::vec3* normalData = new glm::vec3[meshVerts];
		glm::vec3* uvData = new glm::vec3[meshVerts];  // The uv data has 3 components, for cubemapped meshes?
		uint32* indexData = new uint32[meshIndices];

		memcpy(positionData, mesh->mVertices, sizeof(glm::vec3) * meshVerts);
		
		if (mesh->HasNormals())
			memcpy(normalData, mesh->mNormals, sizeof(glm::vec3) * meshVerts);

		if (mesh->HasTextureCoords(0))
			memcpy(uvData, mesh->mTextureCoords[0], sizeof(glm::vec3) * meshVerts);

		// Fetch indexes
		for(uint32 faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			const aiFace& face = mesh->mFaces[faceIndex];
			Ensure(face.mNumIndices == 3);  // Right now we only support triangles

			memcpy(indexData + 3 * faceIndex, face.mIndices, sizeof(uint32) * 3);
		}

		Rendering::MeshData data;

		glGenVertexArrays( 1, &data.VertexObject );
		glGenBuffers( 4, data.Buffers);

		// Upload and bind all the buffers
		{
			glBindVertexArray( data.VertexObject );

			// Upload position
			glBindBuffer( GL_ARRAY_BUFFER, data.Buffers[0] );
			glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * meshVerts, &positionData[0], GL_STATIC_DRAW );
			glEnableVertexAttribArray( 0 );
			glVertexAttribPointer( 0, 3, GL_FLOAT, false, 0, 0 );

			// Upload normals
			glBindBuffer( GL_ARRAY_BUFFER, data.Buffers[1] );
			glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * meshVerts, &normalData[0], GL_STATIC_DRAW );
			glEnableVertexAttribArray( 1 );
			glVertexAttribPointer( 1, 3, GL_FLOAT, false, 0, 0 );

			// Upload UV
			glBindBuffer( GL_ARRAY_BUFFER, data.Buffers[2] );
			glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * meshVerts, &uvData[0], GL_STATIC_DRAW );
			glEnableVertexAttribArray( 2 );
			glVertexAttribPointer( 2, 2, GL_FLOAT, false, sizeof( glm::vec3 ), 0 );

			// Upload UV
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, data.Buffers[3] );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( uint32 ) * meshIndices, &indexData[0], GL_STATIC_DRAW );

			glBindVertexArray( 0 );
		}

		data.DrawMode = GL_TRIANGLES;
		data.DrawCount = meshIndices;
		data.UseElements = true;

		m_Data.push_back(data);
	}

	/* *** LOAD MESH TRANSFORMS *** */
	{
		struct
		{
			void LoadNodeRecursive(aiNode* node, const glm::mat4& transform, std::vector<Rendering::MeshData>& meshList)
			{
				glm::mat4 newTransform = transform * transpose((*(glm::mat4*)&node->mTransformation));

				// Set transforms of owned meshes
				for(uint32 i=0; i<node->mNumMeshes; ++i)
				{
					meshList[node->mMeshes[i]].Transform = newTransform;
				}

				// Do children
				for(uint32 i=0; i<node->mNumChildren; ++i)
				{
					LoadNodeRecursive(node->mChildren[i], newTransform, meshList);
				}
			}
		} nodeLoadHelper;

		nodeLoadHelper.LoadNodeRecursive(scene->mRootNode, glm::mat4(), m_Data);
	}

	return true;
}

/**	Release
*******************************************************************************/
void MeshResource::Release()
{
	for(Rendering::MeshData& data : m_Data)
	{
		glDeleteVertexArrays(1, &data.VertexObject);
		glDeleteBuffers(4, data.Buffers);
	}

	m_Data.clear();
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

	value = ResourceManager::GetInstance()->Load<MeshResource>(resourceName);
	return true;
}