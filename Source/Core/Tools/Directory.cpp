#include "CorePCH.h"
#include "Directory.h"
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

/**	Clean Path
*******************************************************************************/
const std::string FDirectory::CleanPath( const char* path )
{
	std::string result = path;
	bool isDirectory = true;

	for ( int i = 0; i < result.length(); ++i )
	{
		// Replace backslashes
		if ( result[i] == '\\' )
			result[i] = '/';

		// Remove multiple slashes
		if ( result[i] == '/' )
		{
			while ( i + 1 < result.length() && result[i + 1] == '/' || result[i + 1] == '\\' )
			{
				result.erase( i + 1, 1 );
			}

			isDirectory = true;
		}

		if ( result[i] == '.' )
		{
			isDirectory = false;
		}
	}

	// Remove leading slashes
	while ( !result.empty() && result[0] == '/' )
	{
		result.erase( 0, 1 );
	}

	// Add trailing slash (for directories)
	if ( !result.empty() && isDirectory && result[result.length() - 1] != '/' )
	{
		result += "/";
	}

	return result;
}

/**	Constructor
*******************************************************************************/
FDirectory::FDirectory()
	: m_Path( "" )
{
}

/**	Build Path constructor
*******************************************************************************/
FDirectory::FDirectory( const char* path )
{
	*this = path;
}

/**	Add operator
*******************************************************************************/
FDirectory FDirectory::operator+( FDirectory&& other ) const
{
	FDirectory result;
	std::string pathResult = m_Path + other.m_Path;

	result.m_Path = pathResult;
	return result;
}

/**	Add operator
*******************************************************************************/
FDirectory FDirectory::operator+( const FDirectory& other ) const
{
	FDirectory result;
	std::string pathResult = m_Path + other.m_Path;

	result.m_Path = pathResult;
	return result;
}

/**	Add operator
*******************************************************************************/
FDirectory FDirectory::operator+( const char* path ) const
{
	return ( *this + FDirectory( path ) );
}

/**	AddEqual operator
*******************************************************************************/
FDirectory& FDirectory::operator+=( const FDirectory& other )
{
	m_Path += other.m_Path;
	return *this;
}

/**	AddEqual operator
*******************************************************************************/
FDirectory& FDirectory::operator+=( FDirectory&& other )
{
	m_Path += other.m_Path;
	return *this;
}

/**	AddEqual operator
*******************************************************************************/
FDirectory& FDirectory::operator+=( const char* path )
{
	*this += FDirectory(path);
	return *this;
}

/**	Is Root
*******************************************************************************/
bool FDirectory::IsRoot() const
{
	return strstr( m_Path.c_str(), ":/" ) != nullptr ||
		strstr( m_Path.c_str(), ":\\" ) != nullptr;
}

/**	Exists
*******************************************************************************/
bool FDirectory::Exists() const
{
	struct stat info;
	if ( !stat( m_Path.c_str(), &info ) )
	{
		return false;
	}

	return (info.st_mode & S_IFDIR) != 0;
}

/**	Assignment with char ptr
*******************************************************************************/
FDirectory& FDirectory::operator=( const char* path )
{
	m_Path = CleanPath( path );
	return *this;
}
