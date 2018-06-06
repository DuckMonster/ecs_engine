#include "CorePCH.h"

#include "TextureResource.h"
#include "SOIL/SOIL.h"
#include "ResourceManager.h"

/**	Load
*******************************************************************************/
bool TextureResource::Load( const FFile& file )
{
	int width, height;
	unsigned char* image = SOIL_load_image(file.GetPath(), &width, &height, nullptr, SOIL_LOAD_RGB);

	if (image == nullptr)
	{
		Debug_Log("Failed to load texture \"%s\": %s", file.GetPath(), SOIL_last_result());
		return false;
	}

	// Flip image on Y-axis
	const int ROW_SIZE = width * 3;
	const int TOTAL_SIZE = width * height * 3;

	uint8* flippedImage = new uint8[TOTAL_SIZE];
	for(int y = 0; y < height; ++y)
	{
		// Copy row [y] into row [height - (y + 1)]
		memcpy(flippedImage + ROW_SIZE * (height - (y + 1)), image + ROW_SIZE * y, ROW_SIZE);
	}

	// We're done with the soil image now
	SOIL_free_image_data(image);

	// Generate it
	glGenTextures(1, &m_Handle);
	glBindTexture(GL_TEXTURE_2D, m_Handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, flippedImage);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] flippedImage;

	return true;
}

/**	Release
*******************************************************************************/
void TextureResource::Release()
{
	if (!IsValid())
		return;

	glDeleteTextures(1, &m_Handle);
	m_Handle = -1;
	m_Width = m_Height = 0;
}

/**	Serialize Overload
*******************************************************************************/
template<>
bool NamedArchive::Serialize<TextureResource*>( const char* name, TextureResource*& value )
{
	const char* resourceName = nullptr;
	if (!Serialize(name, resourceName))
		return false;

	value = ResourceManager::GetInstance()->Load<TextureResource>(resourceName);
	return true;
}
