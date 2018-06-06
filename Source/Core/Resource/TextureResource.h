#pragma once
#include "Resource.h"

class TextureResource : public Resource
{
public:
	bool Load( const FFile& file ) override;
	void Release() override;

	bool IsValid() const { return m_Handle == -1; }
	GLuint GetTextureHandle() const { return m_Handle; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

private:
	int m_Width = 0;
	int m_Height = 0;
	GLuint m_Handle = -1;
};

#include "Core/Serialize/NamedArchive.h"

/**	Mesh Resource Archive Serialization
*******************************************************************************/
template<>
bool NamedArchive::Serialize<TextureResource*>( const char* name, TextureResource*& value );