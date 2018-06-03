#pragma once
#include "Resource.h"
#include "Core/Rendering/RenderData.h"

class ShaderResource;

class MaterialResource : public Resource
{
public:
	bool Load( const FFile& file ) override;
	void Release() override;

	const Rendering::MaterialData& GetData() const { return m_Data; }
	bool IsValid() const { return m_IsValid; }

	void ParseMaterialFile( const FFile& file );

private:
	MaterialResource* m_Parent = nullptr;
	ShaderResource* m_VertexShader = nullptr;
	ShaderResource* m_FragmentShader = nullptr;

	Rendering::MaterialData m_Data;
	bool m_IsValid = false;
};

#include "Core/Serialize/NamedArchive.h"

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<MaterialResource*>( const char* name, MaterialResource*& value );