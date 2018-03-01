#include "CorePCH.h"
#include "ResourceManager.h"

#include "MeshResource.h"
#include "Core/Utils/File.h"


/**	Load
*******************************************************************************/
Resource* ResourceManager::Load(const char* path)
{
	return GetOrCreateResource<Resource>(path);
}

MeshResource* ResourceManager::LoadMesh(const char* path)
{
	return GetOrCreateResource<MeshResource>(path);
}

/**	Release
*******************************************************************************/
void ResourceManager::Release(Resource* resource)
{
	if (!Ensure(resource))
		return;

	Hash::Type hash = resource->GetHash();
	Resource* mapResource = m_ResourceMap[hash];

	if (!Ensure(mapResource != nullptr))
		return;

	Ensure(resource == mapResource);

	m_ResourceMap[hash] = nullptr;
	delete resource;
}
