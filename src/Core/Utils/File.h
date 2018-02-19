#pragma once
#include <sys/stat.h>
#include <sys/types.h>

namespace File
{
	bool ReadFile( const char* filePath, std::string& outString );
	std::string GetFileFromPath( const std::string& fullPath );

	inline time_t GetFileModifiedTime( const std::string& filePath )
	{
		struct stat result;
		if ( stat( filePath.c_str(), &result ) == 0)
		{
			return result.st_atime;
		}
		else
		{
			Ensure( false );
			return 0;
		}
	}
}