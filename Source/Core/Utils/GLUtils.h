#pragma once
#ifndef GAME_HEADLESS
namespace GLUtils
{
	bool ReadFile( const char* filePath, std::string& outString );

	GLuint CreateShaderFromFile( const char* path );
	GLuint CreateShaderFromFiles( const char* vertPath, const char* fragPath );
	GLuint CreateShaderSrc( const char* vertSrc, const char* fragSrc );

	//--------------------------------------------------- Shader Uniform Setters
	__forceinline void SetShaderUniform( GLuint shader, const char* uniform, const float value )
	{
		glUseProgram( shader );
		glUniform1f( glGetUniformLocation( shader, uniform ), value );
	}
	__forceinline void SetShaderUniform( GLuint shader, const char* uniform, const glm::mat4& value )
	{
		glUseProgram( shader );
		glUniformMatrix4fv( glGetUniformLocation( shader, uniform ), 1, false, value_ptr( value ) );
	}
	__forceinline void SetShaderUniform( GLuint shader, const char* uniform, const glm::vec2& value )
	{
		glUseProgram( shader );
		glUniform2fv( glGetUniformLocation( shader, uniform ), 1, value_ptr( value ) );
	}
	__forceinline void SetShaderUniform( GLuint shader, const char* uniform, const glm::vec3& value )
	{
		glUseProgram( shader );
		glUniform3fv( glGetUniformLocation( shader, uniform ), 1, value_ptr( value ) );
	}
	__forceinline void SetShaderUniform( GLuint shader, const char* uniform, const glm::vec4& value )
	{
		glUseProgram( shader );
		glUniform4fv( glGetUniformLocation( shader, uniform ), 1, value_ptr( value ) );
	}
	__forceinline void SetShaderUniform( GLuint shader, const char* uniform, const int value )
	{
		glUseProgram( shader );
		glUniform1i( glGetUniformLocation( shader, uniform ), value );
	}

	bool CreateTextureFromFile( const char* path, GLuint& outHandle );

	void RenderTexture( GLuint texture );
}
#endif