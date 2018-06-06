#pragma once
#include "Resource.h"
#include "Core/Rendering/RenderData.h"

class MeshResource : public Resource
{
public:
	bool Load(const FFile& file) override;
	void Release() override;

	const Rendering::MeshData& GetData() const { return m_Data; }

private:
	Rendering::MeshData m_Data;
	GLuint m_Buffers[4];
};

#include "Core/Serialize/NamedArchive.h"

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<MeshResource*>(const char* name, MeshResource*& value);