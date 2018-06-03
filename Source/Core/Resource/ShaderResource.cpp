#include "CorePCH.h"
#include "ShaderResource.h"
#include "ResourceManager.h"

/**	Load
*******************************************************************************/
bool ShaderResource::Load( const FFile& file )
{
	// Get shader type from extension
	std::string extension = file.GetExtension();
	if (extension == "vert") m_Type = EShaderType::Vertex;
	if (extension == "frag") m_Type = EShaderType::Fragment;

	// If the shader is invalid, we return false because there's no worth in trying to hotreload it
	// The user will have to change the filename
	if (!Ensure(m_Type != EShaderType::Invalid))
	{
		Debug_Log("\"%s\" is not a valid shader type", file.GetFileName());
		return false;
	}

	GLenum glShaderType;
	switch(m_Type)
	{
	case EShaderType::Vertex: glShaderType = GL_VERTEX_SHADER; break;
	case EShaderType::Fragment: glShaderType = GL_FRAGMENT_SHADER; break;
	}

	// Create
	m_Handle = glCreateShader(glShaderType);

	// Read and compile source
	std::string shaderSource;
	file.ReadAll(shaderSource);

	const char* srcPtr = shaderSource.c_str();
	glShaderSource(m_Handle, 1, &srcPtr, nullptr);
	glCompileShader(m_Handle);

	// Get and print results
	GLint success = 0;
	glGetShaderiv( m_Handle, GL_COMPILE_STATUS, &success );

	if (success == GL_FALSE)
	{
		static char LogBuffer[1 << 11];
		glGetShaderInfoLog(m_Handle, 1 << 11, nullptr, LogBuffer);
		Debug_Log("\"%s\" compilation failed:\n%s", file.GetFileName(), LogBuffer);

		Release();
		return false;
	}

	return true;
}

/**	Release
*******************************************************************************/
void ShaderResource::Release()
{
	if (!IsValid())
		return;

	glDeleteShader(m_Handle);

	m_Handle = -1;
	m_Type = EShaderType::Invalid;
}

/**	Serialize overload
*******************************************************************************/
template<>
bool NamedArchive::Serialize( const char* name, ShaderResource*& value )
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->Load<ShaderResource>(resourceName);
	return true;
}