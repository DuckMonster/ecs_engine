#include "CorePCH.h"
#include "File.h"
#include <fstream>
#include <cstdio>
#include <stack>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include "Directory.h"

using namespace std;

/**	Get File From Path
*******************************************************************************/
std::string FFile::GetFileNameFromPath( const char* path )
{
	int lastSlash = 0;
	int pathLen = (int)strlen( path );
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

	// No file here, return empty
	if ( !containsFile )
		return std::string();

	// No slashes so just return all of it
	if (lastSlash == 0)
		return path;

	return std::string( path + lastSlash + 1 );
}

/**	Constructor
*******************************************************************************/
FFile::FFile()
{
}

/**	Constructor char*
*******************************************************************************/
FFile::FFile( const char* path )
{
	*this = path;
}

/**	Constructor
*******************************************************************************/
FFile::FFile( const std::string& path )
{
	*this = path;
}

/**	Exists
*******************************************************************************/
bool FFile::Exists() const
{
	bool exists = false;
	FILE* file;

	fopen_s(&file, m_Path.c_str(), "r");
	exists = file != nullptr;

	// Remember to close it :)
	if (exists)
		fclose(file);

	return exists;
}

/**	Read All
*******************************************************************************/
bool FFile::ReadAll( std::string& outContent ) const
{
	if (!Ensure(Exists()))
		return false;

	// Open file
	static char buffer[1 << 14];
	ifstream fileStream(m_Path.c_str(), ifstream::binary);
	if (!Ensure(fileStream.is_open()))
		return false;

	// Read whole stream
	fileStream.read(buffer, sizeof(buffer));

	// Add null-terminator
	buffer[(uint32)fileStream.gcount()] = '\0';

	outContent = buffer;
	return true;
}

/**	Get Modified Time
*******************************************************************************/
time_t FFile::GetModifiedTime() const
{
	struct stat result;
	if (stat(m_Path.c_str(), &result) == 0)
	{
		return result.st_mtime;
	}
	else
	{
		// Some error happened, probably it's currently being saved, just return 0
		return 0;
	}
}

/**	Get File Name
*******************************************************************************/
std::string FFile::GetFileName() const
{
	return GetFileNameFromPath(m_Path.c_str());
}

/**	Equals char*
*******************************************************************************/
FFile& FFile::operator=( const char* path )
{
	// When setting a filepath, we want to evaluate the exact path based on current directory
	FDirectory fileDir(path);
	std::string fileName = fileDir.GetExtractedFileName();

	FDirectoryScope scope(fileDir);
	fileName = FDirectory::GetCurrentDirectory().ToString() + fileName;

	m_Path = fileName;
	return *this;
}

/**	Comparison with path
*******************************************************************************/
bool FFile::operator==( const char* path ) const
{
	return m_Path == FDirectory::CleanPath(path);
}