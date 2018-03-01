#pragma once
#include <unordered_map>
#include "Common/hash.h"
#include "Core/Utils/SingletonClass.h"
#include "Core/Utils/Time.h"

class Resource;
class MeshResource;

class ResourceManager : public SingletonClass<ResourceManager>
{
public:
	Resource* Load(const char* path);
	MeshResource* LoadMesh(const char* path);
	void Release(Resource* resource);

private:
	template<typename TResource>
	TResource* GetOrCreateResource(const char* path);

	std::unordered_map<Hash::Type, Resource*> m_ResourceMap;
};

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

	if (!Ensure(resource->Load(path)))
	{
		delete resource;
		return nullptr;
	}

	Debug_Log("\"%s\" loaded (%f seconds)", path, timePoint.Elapsed());

	m_ResourceMap[hash] = resource;

	return resource;
}
