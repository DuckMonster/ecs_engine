#pragma once
#define UNIFORM_SET_H(type) void Set(const char* uniform, const type& value) { Set( uniform, &value, 1 ); }\
void Set( const char* uniform, const type* ptr, const uint count )

enum class MaterialRenderMode
{
	Forward,
	Deferred
};

class CMaterial
{
#ifndef GAME_HEADLESS
public:
	CMaterial();
	~CMaterial();

	void Unload();

	void LoadSource( const char* vertSrc, const char* fragSrc );

	void LoadFromFile( const char* path );
	void LoadFromFile( const char* vertPath, const char* fragPath );

	void LoadTexture( const char* path );
	GLuint GetTexture() const { return m_Texture; }
	bool HasTexture() const { return m_Texture != -1; }

	GLuint GetProgramHandle() const { return m_Program; }

	MaterialRenderMode m_RenderMode = MaterialRenderMode::Deferred;
	bool m_CastsShadow = true;

private:
	GLuint	m_Program = -1;
	GLuint	m_Texture = -1;


#else
public:
	CMaterial() {};

	void Unload() {};

	void LoadSource( const char* vertSrc, const char* fragSrc ) {};

	void LoadFromFile( const char* path ) {};
	void LoadFromFile( const char* vertPath, const char* fragPath ) {};

	void LoadTexture( const char* path ) {};
	GLuint GetTexture() const { return 0; }
	bool HasTexture() const { return true; }

	GLuint GetProgramHandle() const { return 0; }

	MaterialRenderMode m_RenderMode = MaterialRenderMode::Deferred;
	bool m_CastsShadow = true;
#endif

public:
	UNIFORM_SET_H( float );
	UNIFORM_SET_H( int );
	UNIFORM_SET_H( glm::vec2 );
	UNIFORM_SET_H( glm::vec3 );
	UNIFORM_SET_H( glm::vec4 );
	UNIFORM_SET_H( glm::mat4 );
};