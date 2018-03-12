#pragma once
namespace Hash
{
	typedef unsigned int Type;

	const Type FNV_PRIME = 0x01000193;
	const Type FNV_SEED = 0x811C9DC5;

	inline Type FNV( unsigned char byte, Type hash = FNV_SEED )
	{
		return ( byte ^ hash ) * FNV_PRIME;
	}

	inline Type FNV( const void* data, size_t size, Type hash = FNV_SEED )
	{
		unsigned char* cData = (unsigned char*)data;
		while ( size-- )
		{
			hash = FNV( *cData++, hash );
		}

		return hash;
	}

	template<typename T>
	inline Type FNV( const T& data ) { return FNV( &data, sizeof( T ) ); }
}