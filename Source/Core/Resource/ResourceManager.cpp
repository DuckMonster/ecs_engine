#include "CorePCH.h"
#include "ResourceManager.h"

#include "MeshResource.h"
#include "ScriptResource.h"
#include "Core/Utils/File.h"
#include "Core/Utils/Time.h"


/**	Load
*******************************************************************************/
Resource* ResourceManager::Load(const char* path)
{
	return GetOrCreateResource<Resource>(path);
}

/**	Load Mesh
*******************************************************************************/
MeshResource* ResourceManager::LoadMesh(const char* path)
{
	return GetOrCreateResource<MeshResource>(path);
}

/**	Load Script
*******************************************************************************/
ScriptResource* ResourceManager::LoadScript(const char* path)
{
	return GetOrCreateResource<ScriptResource>(path);
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

	// Remove from hotreloading
	{
		std::vector<Resource*>::iterator it = std::find(m_HotloadResources.begin(), m_HotloadResources.end(), resource);
		if (it != m_HotloadResources.end())
			m_HotloadResources.erase(it);
	}

	delete resource;
}

/**	Update Resource Hotloading
*******************************************************************************/
void ResourceManager::UpdateResourceHotReloading()
{
	m_HotReloadTimer.Update(Time::FrameDelta());
	if (m_HotReloadTimer.IsDone())
	{
		m_HotReloadTimer.Reset();

		std::vector<Resource*> resourcesToReload;

		for (Resource* res : m_HotloadResources)
		{
			if (res->HasChanged())
				resourcesToReload.push_back(res);
		}

		for (Resource* res : resourcesToReload)
		{
			Debug_Log("Hot-reloading \"%s\"...", res->GetPath());
			res->HotReload();
		}
	}
}
