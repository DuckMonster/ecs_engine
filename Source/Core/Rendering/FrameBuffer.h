#pragma once

namespace Rendering
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void CreateDownScaled( int scale );
		void Create( int width, int height );
		void Release();

		GLuint GetHandle() const { return m_Handle; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		GLuint AddDepthTexture();
		GLuint AddColorTexture( int components, bool hdr );
		GLuint ColorTextureCount() const { return (GLuint)m_Textures.size(); }

		GLuint GetDepthTexture() const { return m_DepthTexture; }
		GLuint GetColorTexture( uint32 index ) const { return m_Textures[index]; }

		bool IsComplete() const;

	protected:

	private:
		int m_Width, m_Height;
		GLuint m_Handle = -1;

		std::vector<GLuint> m_Textures;
		GLuint m_DepthTexture = -1;
	};

	class FrameBufferScope
	{
	private:
		static FrameBufferScope* CurrentScope;

	public:
		FrameBufferScope( const FrameBuffer& buffer );
		~FrameBufferScope();

	private:
		void Bind();
		void Release();

		const FrameBuffer& m_FrameBuffer;
		FrameBufferScope* m_Previous;
	};
}