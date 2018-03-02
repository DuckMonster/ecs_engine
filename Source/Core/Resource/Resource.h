#pragma once
#include "Core/Delegate/DelegateDispatcher.h"

class ResourceManager;

class Resource
{
public:
	typedef Hash::Type guid_t;

public:
	Resource(ResourceManager* manager, guid_t hash);
	virtual ~Resource();

	virtual bool Load(const char* path);
	virtual void Release();
	virtual void HotReload();

	const ResourceManager* GetManager() const { return m_Manager; }
	const char* GetPath() const { return m_Path.c_str(); }
	const guid_t& GetHash() const { return m_Hash; }

	time_t GetModifiedTime() const;
	bool HasChanged();

	bool Equals(const Resource* other) const;

	DelegateDispatcher<void, Resource*> m_OnHotReloaded;
	DelegateDispatcher<void, Resource*> m_OnReleased;

private:
	ResourceManager* const m_Manager;
	std::string m_Path;
	const guid_t m_Hash;

	time_t m_ModifiedTime;
};

#include "Core/Serialize/NamedArchive.h"

/**	Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<Resource*>(const char* name, Resource*& value);
