#include "CorePCH.h"
#include "FrameBuffer.h"
#include "Core/Context/Context.h"

using namespace Rendering;

namespace
{
	const GLenum DRAW_BUFFER_LIST[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5
	};
}

/**	Constructor
*******************************************************************************/
FrameBuffer::FrameBuffer()
{
}

/**	Destructor
*******************************************************************************/
FrameBuffer::~FrameBuffer()
{
	Release();
}

/**	Create
*******************************************************************************/
void Rendering::FrameBuffer::CreateDownScaled( int scale )
{
	Create(Context::GetInstance()->width / scale, Context::GetInstance()->height / scale);
}

/**	Create
*******************************************************************************/
void FrameBuffer::Create( int width, int height )
{
	Release();

	m_Width = width;
	m_Height = height;

	glGenFramebuffers( 1, &m_Handle );
	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
}

/**	Release
*******************************************************************************/
void FrameBuffer::Release()
{
	if ( m_Handle == -1 )
		return;

	glDeleteFramebuffers( 1, &m_Handle );
	m_Handle = -1;
}

/**	Add Color Texture
*******************************************************************************/
GLuint FrameBuffer::AddColorTexture( int components, bool hdr )
{
	size_t textureIndex = m_Textures.size();
	GLenum intType, extType;

	// Only 1-4 components are supported
	if ( !Ensure( components >= 1 && components <= 4 ) )
		return -1;

	//--------------------------------------------------- Get internal and external data types for the texture
	switch ( components )
	{
	case 1: intType = extType = GL_RED; break;
	case 2: intType = extType = GL_RG; break;
	case 3: intType = extType = GL_RGB; break;
	case 4: intType = extType = GL_RGBA; break;
	}

	// For internal, we check if this is a floating point texture
	if ( hdr )
	{
		switch ( components )
		{
		case 1: intType = GL_R16F; break;
		case 2: intType = GL_RG16F; break;
		case 3: intType = GL_RGB16F; break;
		case 4: intType = GL_RGBA16F; break;
		}
	}

	//--------------------------------------------------- Create and bind texture to framebuffer
	GLuint texture;
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, intType, m_Width, m_Height, 0, extType, hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, nullptr );

	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)textureIndex, GL_TEXTURE_2D, texture, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	m_Textures.push_back( texture );
	return texture;
}

/**	Add Depth Texture
*******************************************************************************/
GLuint FrameBuffer::AddDepthTexture()
{
	if ( m_DepthTexture != -1 )
		return m_DepthTexture;

	GLuint texture;
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr );

	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	m_DepthTexture = texture;
	return texture;
}

/**	Is Complete
*******************************************************************************/
bool Rendering::FrameBuffer::IsComplete() const
{
	if (m_Handle == -1)
		return false;

	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
	bool isComplete = glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return isComplete;
}

//--------------------------------------------------- Frame Buffer Scope
FrameBufferScope* FrameBufferScope::CurrentScope = nullptr;

/**	Constructor
*******************************************************************************/
Rendering::FrameBufferScope::FrameBufferScope( const FrameBuffer& buffer )
	: m_FrameBuffer( buffer ), m_Previous( CurrentScope )
{
	CurrentScope = this;
	Bind();
}

/**	Destructor
*******************************************************************************/
Rendering::FrameBufferScope::~FrameBufferScope()
{
	CurrentScope = m_Previous;
	Release();
}

/**	Bind
*******************************************************************************/
void Rendering::FrameBufferScope::Bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBuffer.GetHandle() );
	glViewport( 0, 0, m_FrameBuffer.GetWidth(), m_FrameBuffer.GetHeight() );
	glDrawBuffers(m_FrameBuffer.ColorTextureCount(), DRAW_BUFFER_LIST);
}

/**	Release
*******************************************************************************/
void Rendering::FrameBufferScope::Release()
{
	if ( m_Previous )
	{
		m_Previous->Bind();
	}
	else
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		glViewport( 0, 0, Context::GetInstance()->width, Context::GetInstance()->height );
		glDrawBuffers(1, DRAW_BUFFER_LIST);
	}
}