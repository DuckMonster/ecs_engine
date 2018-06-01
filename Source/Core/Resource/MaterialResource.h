#pragma once
#include "Resource.h"
#include "Core/Rendering/RenderData.h"

class MaterialResource : public Resource
{
	struct MaterialSerializedData
	{
		std::string vertexFile;
		std::string fragmentFile;

		std::vector<Rendering::UniformValue> uniforms;
	};

public:
	MaterialResource( ResourceManager* manager, guid_t hash ) : Resource( manager, hash ) {}

	bool Load( const char* path ) override;
	void Release() override;

	const Rendering::MaterialData& GetData() const { return m_Data; }

	bool IsValid() const { return m_IsValid; }

	void ParseMaterialFile( const char* file, MaterialSerializedData& outData );

private:
	Rendering::MaterialData m_Data;
	bool m_IsValid = false;
};

#include "Core/Serialize/NamedArchive.h"

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<MaterialResource*>( const char* name, MaterialResource*& value );