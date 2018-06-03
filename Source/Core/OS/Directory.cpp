#include "CorePCH.h"
#include "Directory.h"
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

FDirectory FDirectory::LocalDirectory;
std::vector<FDirectory> FDirectory::DirectoryStack;

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

	// Add trailing slash (for directories)
	if ( !result.empty() && isDirectory && result[result.length() - 1] != '/' )
	{
		result += "/";
	}

	return result;
}

/**	Remove File
*******************************************************************************/
const std::string FDirectory::RemoveFile( const char* path, std::string* outFile /*= nullptr*/ )
{
	int lastSlash = 0;
	int pathLen = strlen( path );
	bool containsFile = false;

	for ( int i = 0; i < pathLen; ++i )
	{
		if ( path[i] == '\\' || path[i] == '/' )
		{
			lastSlash = i;
			containsFile = false;
		}

		else if ( path[i] == '.' )
		{
			containsFile = true;
		}
	}

	// Copy filename to outFile
	if ( outFile != nullptr && containsFile )
	{
		if (lastSlash == 0)
			*outFile = path;
		else
			*outFile = path + lastSlash + 1;
	}

	// Copy rest to result
	if ( !containsFile )
		return path;

	// No slashes so just return nothing
	if (lastSlash == 0)
		return std::string();

	return std::string( path, path + lastSlash + 1 );
}

/**	Get Working Directory
*******************************************************************************/
FDirectory FDirectory::GetWorkingDirectory()
{
	static char DirBuffer[256];
	getcwd( DirBuffer, 256 );

	return FDirectory( DirBuffer );
}

/**	Set Local Directory
*******************************************************************************/
void FDirectory::SetLocalDirectory( FDirectory localDirectory )
{
	LocalDirectory = localDirectory;
}

/**	Get Local Directory
*******************************************************************************/
const FDirectory& FDirectory::GetLocalDirectory()
{
	return LocalDirectory;
}

/**	Get Current Directory
*******************************************************************************/
const FDirectory FDirectory::GetCurrentDirectory()
{
	FDirectory currentDirectory = LocalDirectory;

	// Keep appending/replacing
	for ( FDirectory& dir : DirectoryStack )
	{
		switch ( dir.GetType() )
		{
		case EDirectoryType::Relative:
			// Relative, just add
			currentDirectory += dir;
			break;

		case EDirectoryType::LocalRoot:
			// Local root, relative from LocalDirectory
			currentDirectory = LocalDirectory + dir;
			break;

		case EDirectoryType::SystemRoot:
			// SystemRoot, completely new scope
			currentDirectory = dir;
		}
	}

	return currentDirectory;
}

/**	Push Directory
*******************************************************************************/
void FDirectory::PushDirectory( FDirectory dir )
{
	DirectoryStack.push_back( dir );
}

/**	Pop Directory
*******************************************************************************/
void FDirectory::PopDirectory()
{
	DirectoryStack.pop_back();
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
FDirectory FDirectory::operator+( const FDirectory& other ) const
{
	FDirectory result;

	// You can't add system-roots together
	if ( Ensure( other.GetType() != EDirectoryType::SystemRoot ) )
	{
		result.m_Path = CleanPath( ( m_Path + other.m_Path ).c_str() );
	}

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
	*this = ( *this + other );
	return *this;
}

/**	AddEqual operator
*******************************************************************************/
FDirectory& FDirectory::operator+=( const char* path )
{
	*this += FDirectory( path );
	return *this;
}

/**	Equal
*******************************************************************************/
bool FDirectory::operator==( const FDirectory& other ) const
{
	return m_Path == other.m_Path;
}

/**	Equal
*******************************************************************************/
bool FDirectory::operator==( const char* path ) const
{
	return *this == FDirectory( path );
}

/**	Get Type
*******************************************************************************/
EDirectoryType FDirectory::GetType() const
{
	if ( m_Path[0] == '/' )
		return EDirectoryType::LocalRoot;
	else if ( strstr( m_Path.c_str(), ":/" ) != nullptr )
		return EDirectoryType::SystemRoot;
	else
		return EDirectoryType::Relative;
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

	return ( info.st_mode & S_IFDIR ) != 0;
}

/**	Assignment with char ptr
*******************************************************************************/
FDirectory& FDirectory::operator=( const char* path )
{
	// Clean, remove file, and save it to a variable
	m_Path = RemoveFile( CleanPath( path ).c_str(), &m_ExtractedFile );
	return *this;
}