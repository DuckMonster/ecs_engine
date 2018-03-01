#include "CorePCH.h"
#include "Resource.h"
#include "Core/Utils/File.h"

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
	m_ModifiedTime = GetModifiedTime();
	return true;
}

/**	Get Modified Time
*******************************************************************************/
time_t Resource::GetModifiedTime() const
{
	if (!File::GetFileExists(m_Path))
		return m_ModifiedTime;

	time_t time = File::GetFileModifiedTime(m_Path);
	return time != 0 ? time : m_ModifiedTime;
}

/**	Has Changed
*******************************************************************************/
bool Resource::HasChanged()
{
	time_t oldTime = m_ModifiedTime;
	m_ModifiedTime = GetModifiedTime();

	return oldTime < m_ModifiedTime;
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
