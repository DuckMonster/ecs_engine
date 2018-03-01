#pragma once
#include "Resource.h"

class MeshResource : public Resource
{
public:
	MeshResource(ResourceManager* manager, Hash::Type hash) : Resource(manager, hash) {}

	bool Load(const char* path) override;

	GLuint GetVAO() const { return m_VAO; }
	uint32 GetDrawCount() const { return m_DrawCount; }
	GLenum GetDrawMode() const { return m_DrawMode; }
	bool GetUsingElements() const { return m_Elements; }

private:
	GLuint m_VAO;
	uint32 m_DrawCount;
	GLenum m_DrawMode;
	bool m_Elements;
};

#include "Core/Serialize/NamedArchive.h"
#include "ResourceManager.h"

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
inline bool NamedArchive::Serialize<MeshResource*>(const char* name, MeshResource*& value)
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->LoadMesh(resourceName);
	return true;
}