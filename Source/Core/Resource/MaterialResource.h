#pragma once
#include "Resource.h"
#include "Core/Serialize/NamedArchive.h"
#include "Core/Rendering/RenderData.h"

class ShaderResource;
class TextureResource;

class MaterialResource : public Resource
{
public:
	bool Load( const FFile& file ) override;
	void Release() override;

	const Rendering::MaterialData& GetData() const { return m_Data; }
	bool IsValid() const { return m_IsValid; }

private:
	void ParseMaterialFile( const FFile& file );
	void ParseUniforms( NamedArchive& archive );

	MaterialResource* m_Parent = nullptr;
	ShaderResource* m_VertexShader = nullptr;
	ShaderResource* m_FragmentShader = nullptr;
	std::vector<TextureResource*> m_Textures;

	Rendering::MaterialData m_Data;
	bool m_IsValid = false;

	std::vector<Rendering::UniformValue> m_UniformValues;
};

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<MaterialResource*>( const char* name, MaterialResource*& value );