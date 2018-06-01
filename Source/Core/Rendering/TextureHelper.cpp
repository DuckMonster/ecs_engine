#include "CorePCH.h"
#include "TextureHelper.h"

TextureHelper::TextureHelper() { }
TextureHelper::~TextureHelper() { }

void TextureHelper::Bind( GLuint texture, uint32 index )
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
}
