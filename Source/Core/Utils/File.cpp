#include "CorePCH.h"
#include "File.h"
#include <fstream>
#include <cstdio>

using namespace std;

/**	Read File
*******************************************************************************/
bool File::ReadFile(const char* filePath, string& outString)
{
	static char buffer[1024];

	// Open file
	ifstream fileStream(filePath, ifstream::binary);
	if (!Ensure(fileStream.is_open()))
		return false;

	// Read whole stream
	fileStream.read(buffer, sizeof(buffer));

	// Add null-terminator
	buffer[(uint32)fileStream.gcount()] = '\0';

	outString = buffer;
	return true;
}

/**	Trim To File
*******************************************************************************/
string File::GetFileFromPath(const string& fullPath)
{
	int lastSlash = -1;
	for (uint32 i = 0; i < fullPath.length(); i++)
	{
		if (fullPath[i] == '\\' || fullPath[i] == '/')
			lastSlash = i;
	}

	return fullPath.substr(lastSlash + 1);
}

bool File::GetFileExists(const std::string& path)
{
	FILE* file = fopen(path.c_str(), "r");
	if (file == nullptr)
		return false;

	fclose(file);
	return true;
}
