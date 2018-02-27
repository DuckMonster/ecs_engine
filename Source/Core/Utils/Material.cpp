#include "CorePCH.h"
#include "Material.h"
#include "File.h"
#include "GLUtils.h"

#ifndef GAME_HEADLESS
#define UNIFORM_SET_CPP(type, func, valptr) \
void CMaterial::Set(const char* uniform, const type* value, uint32 count) \
{ \
	glUseProgram(m_Program); \
	GLuint handle = glGetUniformLocation(m_Program, uniform); \
	\
	if ( !EnsureMsg(handle != GLuint(-1), "Uniform \"%s\" wasn't found", uniform) ) \
		return; \
	\
	func( handle, count, (valptr*)value);\
}

#define UNIFORM_SET_MAT_CPP(type, func, valptr) \
void CMaterial::Set(const char* uniform, const type* value, uint32 count) \
{ \
	glUseProgram(m_Program); \
	GLuint handle = glGetUniformLocation(m_Program, uniform); \
	\
	if ( !EnsureMsg(handle != GLuint(-1), "Uniform \"%s\" wasn't found", uniform) ) \
		return; \
	\
	func( handle, count, false, (valptr*)value);\
}
#else
#define UNIFORM_SET_CPP(type, func, valptr) \
void CMaterial::Set(const char* uniform, const type* value, uint count) {}
#define UNIFORM_SET_MAT_CPP(type, func, valptr) \
void CMaterial::Set(const char* uniform, const type* value, uint count) {}
#endif

using namespace std;
#ifndef GAME_HEADLESS
/**	Constructor
*******************************************************************************/
CMaterial::CMaterial()
{
}

/**	Destructor
*******************************************************************************/
CMaterial::~CMaterial()
{
	Unload();
}

/**	Un-load
*******************************************************************************/
void CMaterial::Unload()
{
	if ( m_Program == -1 )
		return;

	glDeleteProgram( m_Program );
	m_Program = -1;
}

/**	Load Source
*******************************************************************************/
void CMaterial::LoadSource( const char* vertSrc, const char* fragSrc )
{
	Unload();
	m_Program = GLUtils::CreateShaderSrc( vertSrc, fragSrc );
}

/**	Load Shader
*******************************************************************************/
void CMaterial::LoadFromFile( const char* path )
{
	string vertPath( path ),
		fragPath( path );

	vertPath += ".vert";
	fragPath += ".frag";

	LoadFromFile( vertPath.c_str(), fragPath.c_str() );
}

/**	Load Shaders
*******************************************************************************/
void CMaterial::LoadFromFile( const char* vertPath, const char* fragPath )
{
	string vertSrc, fragSrc;

	if ( !Ensure( File::ReadFile( vertPath, vertSrc ) ) )
	{
		Debug_Log( "Failed to load file \"%s\"", vertPath );
		return;
	}

	if ( !Ensure( File::ReadFile( fragPath, fragSrc ) ) )
	{
		Debug_Log( "Failed to load file \"%s\"", fragPath );
		return;
	}

	Debug_Log( "Loading shader \"%s\" / \"%s\"", File::GetFileFromPath( vertPath ).c_str(), File::GetFileFromPath( fragPath ).c_str() );

	LoadSource( vertSrc.c_str(), fragSrc.c_str() );
}

/**	Load Texture
*******************************************************************************/
void CMaterial::LoadTexture( const char* path )
{
	GLUtils::CreateTextureFromFile( path, m_Texture );
}
#endif

UNIFORM_SET_CPP( float, glUniform1fv, float )
UNIFORM_SET_CPP( glm::vec2, glUniform2fv, float )
UNIFORM_SET_CPP( glm::vec3, glUniform3fv, float )
UNIFORM_SET_CPP( glm::vec4, glUniform4fv, float )
UNIFORM_SET_MAT_CPP( glm::mat4, glUniformMatrix4fv, float )
UNIFORM_SET_CPP( int, glUniform1iv, int )