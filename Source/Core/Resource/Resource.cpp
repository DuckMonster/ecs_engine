#include "CorePCH.h"
#include "Resource.h"
#include "Core/OS/File.h"
#include "../OS/Directory.h"
#include "../Tools/Time.h"

/**	Destructor
*******************************************************************************/
Resource::~Resource()
{
}

/**	Has Changed
*******************************************************************************/
bool Resource::ShouldReload()
{
	if (m_File.GetModifiedTime() != m_LastModifiedTime)
	{
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
	Ensure(m_File == other->m_File);
#endif

	return isEquals;
}

/**	Add Dependency
*******************************************************************************/
void Resource::AddDependency( Resource* other )
{
	if (m_Dependencies.find(other) != m_Dependencies.end())
		return;

	m_Dependencies.insert(other);
	other->AddDependentResource(this);
}

/**	Load Internal
*******************************************************************************/
bool Resource::Load_Internal( const FFile& file )
{
	// Handles tab formatting when loading resource dependencies, so it's easier to read :)
	static int Recursion = 0;

	char indentString[20];
	for(int i=0; i<Recursion; ++i)
		indentString[i] = '\t';

	indentString[Recursion] = '\0';

	Recursion++;
	//

	bool firstTimeLoad = (m_File == "");

	Debug_Log("%s%s \"%s\"...", indentString, firstTimeLoad ? "Loading" : "Hot-reloading", file.GetFileName());
	FTimePoint timePoint;

	FDirectoryScope scope(file.GetPath());

	m_File = file;
	m_LastModifiedTime = file.GetModifiedTime();
	bool result = Load(file);

	for(Resource* dependentResource : m_DependentResources)
		dependentResource->HotReload_Internal();

	Debug_Log("%s\"%s\" %s %s! ( %f seconds )", indentString, file.GetFileName(), firstTimeLoad ? "loading" : "hot-reloading", result ? "SUCCESS" : "FAIL", timePoint.Elapsed());
	
	Recursion--;
	return result;
}

/**	Release Internal
*******************************************************************************/
void Resource::Release_Internal()
{
	Release();

	m_OnReleased();
}

/**	HotReload Internal
*******************************************************************************/
void Resource::HotReload_Internal()
{
	Release_Internal();
	Load_Internal(m_File);

	m_OnHotReloaded();
}

/**	Clear Dependencies
*******************************************************************************/
void Resource::ClearDependencies()
{
	for(Resource* depencency : m_Dependencies)
	{
		depencency->RemoveDependentResource(this);
	}

	m_Dependencies.clear();
}

/**	Add Dependent Resource
*******************************************************************************/
void Resource::AddDependentResource( Resource* other )
{
	// Already dependent
	if (m_DependentResources.find(other) != m_DependentResources.end())
		return;

	m_DependentResources.insert(other);
}

/**	Remove Dependent Resource
*******************************************************************************/
void Resource::RemoveDependentResource( Resource* other )
{
	m_DependentResources.erase(other);
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

	value = ResourceManager::GetInstance()->Load<Resource>(name);
	return true;
}
