#include "CorePCH.h"

#ifndef GAME_HEADLESS
#include "GLUtils.h"
#include <SOIL/SOIL.h>
#include <fstream>
#include "Core/Resource/MaterialResource.h"

using namespace std;

/**	Read File
*******************************************************************************/
bool GLUtils::ReadFile( const char* filePath, string& outString )
{
	static char buffer[0xFFFF];

	// Open file
	ifstream fileStream( filePath, ifstream::binary );
	if (!fileStream.is_open( ))
		return false;

	// Read whole stream
	fileStream.read( buffer, sizeof( buffer ) );

	// Add null-terminator
	buffer[(uint32)fileStream.gcount( )] = '\0';

	outString = buffer;
	return true;
}

/**	Create Shader From File
*******************************************************************************/
GLuint GLUtils::CreateShaderFromFile( const char * path )
{
	Debug_Log( "Loading shader \"%s\"", path );

	std::string vertPath( path ), fragPath( path ),
		vertSrc, fragSrc;

	vertPath += ".vert";
	fragPath += ".frag";

	bool bSuccess = true;

	bSuccess &= ReadFile( vertPath.c_str( ), vertSrc );
	bSuccess &= ReadFile( fragPath.c_str( ), fragSrc );

	if (!bSuccess)
	{
		Debug_Log("Failed to open files for shader \"%s\"", path);
		return -1;
	}

	return CreateShaderSrc( vertSrc.c_str( ), fragSrc.c_str( ) );
}

/**	Create Shader From Files
*******************************************************************************/
GLuint GLUtils::CreateShaderFromFiles( const char * vertPath, const char * fragPath )
{
	Debug_Log( "Loading shaders \"%s\", \"%s\"", vertPath, fragPath );

	std::string vertSrc, fragSrc;

	ReadFile( vertPath, vertSrc );
	ReadFile( fragPath, fragSrc );

	return CreateShaderSrc( vertSrc.c_str( ), fragSrc.c_str( ) );
}

/**	Create Shader From Source
*******************************************************************************/
GLuint GLUtils::CreateShaderSrc( const char* vertSrc, const char* fragSrc )
{
	static char logBuffer[1024];

	GLuint program = glCreateProgram( ),
		vert = glCreateShader( GL_VERTEX_SHADER ),
		frag = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vert, 1, &vertSrc, nullptr );
	glShaderSource( frag, 1, &fragSrc, nullptr );
	glCompileShader( vert );
	glCompileShader( frag );

	glAttachShader( program, vert );
	glAttachShader( program, frag );
	glLinkProgram( program );

	glGetProgramInfoLog( program, 1024, nullptr, logBuffer );
	if (strlen( logBuffer ) > 0)
	{
		Debug_Log( "Shader compile log:\n%s", logBuffer );
	}

	// Clean up
	glDetachShader( program, vert );
	glDetachShader( program, frag );
	glDeleteShader( vert );
	glDeleteShader( frag );

	return program;
}

/**	Create Texture From File
*******************************************************************************/
bool GLUtils::CreateTextureFromFile( const char * path, GLuint& outHandle )
{
	Debug_Log( "Loading texture \"%s\"", path );

	int width, height;
	unsigned char* image = SOIL_load_image( path, &width, &height, nullptr, SOIL_LOAD_RGB );

	if (image == nullptr)
	{
		Debug_Log( "Failed to load \"%s\"", path );
		return false;
	}

	glGenTextures( 1, &outHandle );
	glBindTexture( GL_TEXTURE_2D, outHandle );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	SOIL_free_image_data( image );

	return true;
}

/**	Shader Utils
*******************************************************************************/
namespace
{
	const char* QUAD_SHADER_VERT = ""
		"#version 330 core\n"
		"const vec2 quadVertices[4] = \n"
		"vec2[]( vec2( -1.0, -1.0 ), vec2( 1.0, -1.0 ), vec2( 1.0, 1.0 ), vec2( -1.0, 1.0 ) );\n"

		"const vec2 quadUvs[4] = \n"
		"vec2[]( vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ), vec2( 1.0, 1.0 ), vec2( 0.0, 1.0 ) );\n"

		"out vec2 f_UV;\n"

		"void main( ) {\n"
		"	gl_Position = vec4( quadVertices[gl_VertexID], -1.0, 1.0 );\n"
		"	f_UV = quadUvs[gl_VertexID];\n"
		"}";

	const char* QUAD_SHADER_FRAG = ""
		"#version 330 core\n"
		"uniform sampler2D u_Sampler;"

		"in vec2 f_UV;"
		"out vec4 o_Color;"
		"void main() {"
		"	o_Color = texture(u_Sampler, f_UV);"
		"}";
}

void GLUtils::RenderTexture( GLuint texture, MaterialResource* material/* = nullptr*/ )
{
	static GLuint QuadShader;
	static bool ShaderCompiled = false;

	if (!ShaderCompiled)
	{
		QuadShader = CreateShaderSrc( QUAD_SHADER_VERT, QUAD_SHADER_FRAG );
		ShaderCompiled = true;
	}

	if (material)
		glUseProgram(material->GetData().ShaderHandle);
	else
		glUseProgram( QuadShader );

	glDisable( GL_DEPTH_TEST );
	glBindTexture( GL_TEXTURE_2D, texture );
	glDrawArrays( GL_QUADS, 0, 4 );
}
#endif
