#pragma once
#include "Resource.h"
#include "Core/Rendering/RenderData.h"

enum class EShaderType
{
	Invalid,
	Vertex,
	Fragment
};

class ShaderResource : public Resource
{
public:
	bool Load( const FFile& file ) override;
	void Release() override;

	EShaderType GetType() const { return m_Type; }

	GLuint GetShaderHandle() { return m_Handle; }
	bool IsValid() const { return m_Handle != -1; }

private:
	GLuint m_Handle = -1;

	EShaderType m_Type = EShaderType::Invalid;
};

#include "Core/Serialize/NamedArchive.h"

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<ShaderResource*>( const char* name, ShaderResource*& value );