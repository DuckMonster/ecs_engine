#include "CorePCH.h"
#include "MaterialResource.h"
#include "Core/Tools/GLUtils.h"
#include "ResourceManager.h"
#include "ShaderResource.h"
#include "TextureResource.h"

/**	Load
*******************************************************************************/
bool MaterialResource::Load( const FFile& file )
{
	ParseMaterialFile( file );

	if ( !m_VertexShader || !m_FragmentShader || !m_VertexShader->IsValid() || !m_FragmentShader->IsValid() )
	{
		Debug_Log( "To create a material we need a vertex and fragment shader" );
		return false;
	}

	// Create and link program
	GLuint program = glCreateProgram();
	glAttachShader( program, m_VertexShader->GetShaderHandle() );
	glAttachShader( program, m_FragmentShader->GetShaderHandle() );
	glLinkProgram( program );

	// Get success and show info
	GLint isLinked = 0;
	glGetProgramiv( program, GL_LINK_STATUS, &isLinked );

	if ( isLinked == GL_FALSE )
	{
		static char LogBuffer[1 << 11];
		glGetProgramInfoLog( program, 1 << 11, nullptr, LogBuffer );
		Debug_Log( "Material \"%s\" link failed:\n%s", file.GetFileName(), LogBuffer );
		
		Release();
		return false;
	}

	m_Data.ShaderHandle = program;
	for (TextureResource* texture : m_Textures)
	{
		m_Data.Textures.push_back(texture->GetTextureHandle());
	}

	m_IsValid = true;

	return true;
}

/**	Release
*******************************************************************************/
void MaterialResource::Release()
{
	if ( IsValid() )
		glDeleteProgram( m_Data.ShaderHandle );

	m_Data.ShaderHandle = -1;
	m_Parent = nullptr;
	m_VertexShader = nullptr;
	m_FragmentShader = nullptr;
	m_IsValid = false;
}

/**	Parse Material File
*******************************************************************************/
void MaterialResource::ParseMaterialFile( const FFile& file )
{
	NamedArchive::Source source = NamedArchive::Open( file.GetPath() );
	NamedArchive ar( source );

	const char* parentFile = nullptr;
	ar.Serialize( "Parent", parentFile );

	// Parent material
	if ( parentFile != nullptr )
	{
		m_Parent = GetManager()->Load<MaterialResource>( parentFile );
		if (m_Parent)
			AddDependency(m_Parent);
		else
			Debug_Log( "Failed to load parent material \"%s\"", parentFile );
	}

	// Shaders
	const char* vertexFile = nullptr;
	const char* fragmentFile = nullptr;

	ar.Serialize( "Vertex", vertexFile );
	ar.Serialize( "Fragment", fragmentFile );

	if ( vertexFile != nullptr )
	{
		m_VertexShader = GetManager()->Load<ShaderResource>( vertexFile );
		if ( m_VertexShader )
			AddDependency(m_VertexShader);
		else
			Debug_Log( "Failed to load vertex shader \"%s\"", vertexFile );

	}

	if ( fragmentFile != nullptr )
	{
		m_FragmentShader = GetManager()->Load<ShaderResource>( fragmentFile );
		if (m_FragmentShader)
			AddDependency(m_FragmentShader);
		else
			Debug_Log( "Failed to load fragment shader \"%s\"", fragmentFile );
	}

	// If this resource did not set a vertex or fragment shader, search parents
	MaterialResource* parent = m_Parent;
	while ( ( !m_VertexShader || !m_FragmentShader ) && parent )
	{
		// Circular dependency!!!!
		if ( !Ensure( parent != this ) )
			break;

		if ( !m_VertexShader )
			m_VertexShader = parent->m_VertexShader;
		if ( !m_FragmentShader )
			m_FragmentShader = parent->m_FragmentShader;

		parent = parent->m_Parent;
	}

	// Textures
	std::vector<const char*> textureFiles;
	ar.SerializeArray("Textures", textureFiles);

	for(const char* file : textureFiles)
	{
		TextureResource* texture = GetManager()->Load<TextureResource>(file);
		if (texture != nullptr)
		{
			AddDependency(texture);
			m_Textures.push_back(texture);
		}
	}

	// Parse uniforms
	ParseUniforms(ar.Push("Uniforms"));
}

/**	Parse Uniforms
*******************************************************************************/
void MaterialResource::ParseUniforms( NamedArchive& archive )
{
	int uniformNum = archive.ArraySize();
	for(int i=0; i<uniformNum; ++i)
	{
		NamedArchive uniform = archive.Push(i);
		const char* name = nullptr;

		uniform.Serialize("Name", name);
	}
}

/**	Serialize Specialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize( const char* name, MaterialResource*& value )
{
	const char* resourceName = nullptr;
	if ( !Serialize( name, resourceName ) )
		return false;

	value = ResourceManager::GetInstance()->Load<MaterialResource>( resourceName );
	return true;
}