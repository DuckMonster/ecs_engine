#pragma once

class ResourceManager;

class Resource
{
public:
	Resource(ResourceManager* manager, Hash::Type hash);
	virtual ~Resource();

	virtual bool Load(const char* path);

	const ResourceManager* GetManager() const { return m_Manager; }
	const char* GetPath() const { return m_Path.c_str(); }
	const Hash::Type& GetHash() const { return m_Hash; }

	time_t GetModifiedTime() const;
	bool HasChanged();

	bool Equals(const Resource* other) const;

private:
	ResourceManager* const m_Manager;
	std::string m_Path;
	const Hash::Type m_Hash;

	time_t m_ModifiedTime;
};

#include "Core/Serialize/NamedArchive.h"

/**	Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<Resource*>(const char* name, Resource*& value);
