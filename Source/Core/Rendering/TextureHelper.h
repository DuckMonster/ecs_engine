#pragma once

class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();

	void Bind(GLuint texture, uint32 index);
};