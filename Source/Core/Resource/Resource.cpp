#include "CorePCH.h"
#include "Resource.h"
#include "Core/OS/File.h"

/**	Constructor
*******************************************************************************/
Resource::Resource(ResourceManager* manager, Hash::Type hash) :
	m_Manager(manager),
	m_Hash(hash)
{
}

/**	Destructor
*******************************************************************************/
Resource::~Resource()
{
}

/**	Load
*******************************************************************************/
bool Resource::Load(const char* path)
{
	m_Path = path;
	AddFileDependency(path);
	return true;
}

/**	Release
*******************************************************************************/
void Resource::Release()
{
	m_OnReleased.Broadcast(this);
	m_FileDependencies.clear();
}

/**	Hot Reload
*******************************************************************************/
void Resource::HotReload()
{
	Release();
	Load(m_Path.c_str());
	m_OnHotReloaded.Broadcast(this);
}

/**	Has Changed
*******************************************************************************/
bool Resource::ShouldReload()
{
	for(FFileDependency& dependency : m_FileDependencies)
	{
		time_t modTime = dependency.File.GetModifiedTime();
		if (modTime > dependency.LastModifiedTime)
			return true;
	}

	return false;
}

/**	Equals
*******************************************************************************/
bool Resource::Equals(const Resource* other) const
{
	bool isEquals = true;

	isEquals &= m_Hash == other->m_Hash;
	
#if DEBUG
	// In debug we wanna double-check that the paths are actually equal
	// if this triggers, the hash is not unique enough
	Ensure(m_Path == other->m_Path);
#endif

	return isEquals;
}

/**	Add File Dependency
*******************************************************************************/
void Resource::AddFileDependency( const std::string& path )
{
	FFileDependency dependency;
	dependency.File = path;
	dependency.LastModifiedTime = dependency.File.GetModifiedTime();

	m_FileDependencies.push_back(dependency);
}

#include "ResourceManager.h"
/**	Serialize Specialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<Resource*>(const char* name, Resource*& value)
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->Load(name);
	return true;
}
