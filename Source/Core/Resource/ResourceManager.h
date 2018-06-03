#pragma once
#include <unordered_map>
#include "Common/hash.h"
#include "Core/Tools/SingletonClass.h"
#include "Core/Tools/Time.h"
#include "Core/Tools/Timer.h"
#include "Core/OS/Directory.h"

class Resource;

class ResourceManager : public SingletonClass<ResourceManager>
{
public:
	template<class TResource>
	TResource* Load(const char* path);

	void Release(Resource* resource);
	void UpdateResourceHotReloading();

private:
	std::unordered_map<Hash::Type, Resource*> m_ResourceMap;
	std::vector<Resource*> m_HotloadResources;

	Timer m_HotReloadTimer = Timer(0.5f);
};

/**	Load Resource
*******************************************************************************/
template<class TResource>
TResource* ResourceManager::Load(const char* path)
{
	FFile file(path);

	if (!file.Exists())
	{
		Debug_Log("Resource file \"%s\" doesn't exist", file.GetPath());
		return nullptr;
	}

	Hash::Type hash = Hash::FNV(path, strlen(path));

	// Check if resource is already loaded
	TResource* resource = (TResource*)m_ResourceMap[hash];
	
	if (resource)
	{
#if DEBUG
		// Make sure there are no hash overlaps
		Ensure(file == resource->GetFile());
#endif
		return resource;
	}

	// Load it up!
	resource = new TResource();
	resource->SetManager(this);
	resource->SetHash(hash);
	resource->Load_Internal(file);

	m_ResourceMap[hash] = resource;
	m_HotloadResources.push_back(resource);

	return resource;
}