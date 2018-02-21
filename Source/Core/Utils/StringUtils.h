#pragma once
#include <string.h>
#include <vector>
#include <stdarg.h>

namespace StringUtils
{
	inline void Tokenize( const std::string& inString, const char token, std::vector<std::string>& outVector, const bool ignoreEmpty = false )
	{
		uint p = 0;
		for ( uint i = 0; i <= inString.size(); ++i )
		{
			if ( inString[i] == token || i == inString.size() )
			{
				if ( i - p > 1 || !ignoreEmpty )
					outVector.emplace_back( inString, p, i - p );

				p = i + 1;
				continue;
			}
		}
	}

	inline std::string TrimActorPath( const std::string& path, int actors )
	{
		std::string result = path;

		for ( int i = 0; i < actors; i++ )
		{
			// -2 because ignore any trailing slashes
			size_t offset = result.find_last_of( '/', result.size() - 2 );
			if ( offset == std::string::npos )
				return result;

			result = result.substr( 0, offset );
		}

		return result;
	}

	inline std::string GetLastNameInActorPath( const std::string& path )
	{
		std::string result = path;

	}

	inline std::string Printf( const char* format, ... )
	{
		static char BUFFER[1024];
		va_list vl;
		va_start( vl, format );
		
		vsprintf_s( BUFFER, 1024, format, vl );
		
		va_end( vl );

		return std::string( BUFFER );
	}
}