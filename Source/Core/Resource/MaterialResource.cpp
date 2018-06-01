#include "CorePCH.h"
#include "MaterialResource.h"
#include "Core/Utils/GLUtils.h"
#include "ResourceManager.h"

/**	Load
*******************************************************************************/
bool MaterialResource::Load( const char* path )
{
	Resource::Load(path);

	MaterialSerializedData data;
	ParseMaterialFile(path, data);

	//--------------------------------------------------- Find and compile shaders
	if (data.vertexFile.empty())
	{
		Debug_Log("No vertex shader specified in material \"%s\"", path);
		return true;
	}

	if (data.fragmentFile.empty())
	{
		Debug_Log("No fragment shader specified in material \"%s\"", path);
		return true;
	}

	m_Data.ShaderHandle = GLUtils::CreateShaderFromFiles(data.vertexFile.c_str(), data.fragmentFile.c_str());
	AddFileDependency(data.vertexFile.c_str());
	AddFileDependency(data.fragmentFile.c_str());

	m_IsValid = true;

	//--------------------------------------------------- Get uniform specifications
	// Write me!
}

/**	Release
*******************************************************************************/
void MaterialResource::Release()
{
	if (m_Data.ShaderHandle != -1)
		glDeleteProgram(m_Data.ShaderHandle);

	m_Data.ShaderHandle = -1;
	Resource::Release();

	m_IsValid = false;
}

/**	Parse Material File
*******************************************************************************/
void MaterialResource::ParseMaterialFile( const char* file, MaterialSerializedData& outData )
{
	NamedArchive::Source source = NamedArchive::Open(file);
	NamedArchive ar(source);

	ar.Serialize("Vertex", outData.vertexFile);
	ar.Serialize("Fragment", outData.fragmentFile);
}

/**	Serialize Specialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize( const char* name, MaterialResource*& value )
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->LoadMaterial(resourceName);
	return true;
}