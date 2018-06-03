#pragma once
#include "Core/Delegate/Delegate.h"
#include "Core/OS/File.h"

class ResourceManager;

class Resource
{
private:
	struct FFileDependency
	{
		FFile File;
		time_t LastModifiedTime;
	};

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

	bool ShouldReload();

	bool Equals(const Resource* other) const;

	Delegate<void, Resource*> m_OnHotReloaded;
	Delegate<void, Resource*> m_OnReleased;

protected:
	void AddFileDependency(const std::string& path);

private:
	ResourceManager* const m_Manager;
	std::string m_Path;
	const guid_t m_Hash;

	std::vector<FFileDependency> m_FileDependencies;

	time_t m_ModifiedTime;
};

#include "Core/Serialize/NamedArchive.h"

/**	Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<Resource*>(const char* name, Resource*& value);
