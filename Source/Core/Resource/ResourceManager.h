#pragma once
#include <unordered_map>
#include "Common/hash.h"
#include "Core/Utils/SingletonClass.h"
#include "Core/Utils/Time.h"
#include "Core/Utils/Timer.h"

class Resource;
class MeshResource;
class ScriptResource;
class MaterialResource;

class ResourceManager : public SingletonClass<ResourceManager>
{
public:
	Resource* Load(const char* path);
	MeshResource* LoadMesh(const char* path);
	ScriptResource* LoadScript(const char* path);
	MaterialResource* LoadMaterial(const char* path);
	void Release(Resource* resource);

	void UpdateResourceHotReloading();

private:
	template<typename TResource>
	TResource* GetOrCreateResource(const char* path);

	std::unordered_map<Hash::Type, Resource*> m_ResourceMap;
	std::vector<Resource*> m_HotloadResources;

	Timer m_HotReloadTimer = Timer(0.5f);
};

/**	Get Or Create Resource
*******************************************************************************/
template<typename TResource>
TResource* ResourceManager::GetOrCreateResource(const char* path)
{
	if (!Ensure(File::GetFileExists(path)))
		return nullptr;

	Hash::Type hash = Hash::FNV(path, strlen(path));
	TResource* resource = (TResource*)m_ResourceMap[hash];
	
	if (resource)
	{
#if DEBUG
		Ensure(::strcmp(path, resource->GetPath()) == 0);
#endif
		return resource;
	}

	resource = new TResource(this, hash);
	
	FTimePoint timePoint;
	Debug_Log("Loading resource \"%s\"...", path);

	if (!resource->Load(path))
	{
		Debug_Log("Resource \"%s\" failed to load", path);
		delete resource;
		return nullptr;
	}

	Debug_Log("\"%s\" loaded (%f seconds)", path, timePoint.Elapsed());

	m_ResourceMap[hash] = resource;
	m_HotloadResources.push_back(resource);

	return resource;
}
