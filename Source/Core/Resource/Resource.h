#pragma once
#include "Core/Delegate/Delegate.h"
#include "Core/OS/File.h"
#include <set>

class ResourceManager;

class Resource
{
	friend class ResourceManager;

private:

public:
	typedef Hash::Type guid_t;

public:
	virtual ~Resource();

	virtual bool Load(const FFile& file) { return true; }
	virtual void Release() {}

	ResourceManager* GetManager() const { return m_Manager; }
	const guid_t& GetHash() const { return m_Hash; }
	const FFile& GetFile() const { return m_File; }
	const char* GetPath() const { return m_File.GetPath(); }
	const char* GetFileName() const { return m_File.GetFileName(); }

	bool ShouldReload();

	bool Equals(const Resource* other) const;

	Delegate<void> m_OnHotReloaded;
	Delegate<void> m_OnReleased;

protected:
	void AddDependency(Resource* other);

private:
	/* Internal functions called by ResourceManager */
	void SetManager(ResourceManager* manager) { m_Manager = manager; }
	void SetHash(guid_t hash) { m_Hash = hash; }
	void SetFile(const FFile& file) { m_File = file; }

	bool Load_Internal(const FFile& file);
	void Release_Internal();
	void HotReload_Internal();
	/* ************** */

	void ClearDependencies();
	void AddDependentResource(Resource* other);
	void RemoveDependentResource(Resource* other);

	ResourceManager* m_Manager;
	guid_t m_Hash;

	FFile m_File;

	std::set<Resource*> m_Dependencies;
	std::set<Resource*> m_DependentResources;

	// Used for checking if resource should be hot-reloaded
	time_t m_LastModifiedTime;
};

#include "Core/Serialize/NamedArchive.h"

/**	Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<Resource*>(const char* name, Resource*& value);
