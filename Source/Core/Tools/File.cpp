#include "CorePCH.h"
#include "File.h"
#include <fstream>
#include <cstdio>
#include <stack>
#include <iostream>

using namespace std;

/**	Path Stack
*******************************************************************************/
class PathStack
{
public:
	// Push
	void PushPath(const char* path)
	{
		std::string cleanedPath = File::CleanPath(path);
		cleanedPath = File::RemoveFileFromPath(cleanedPath.c_str());

		m_Stack.push_back(cleanedPath);
	}

	// Pop
	void PopPath()
	{
		m_Stack.erase(m_Stack.begin() + (m_Stack.size() - 1));
	}

	// Get
	std::string ToString()
	{
		std::string result;

		for(const std::string& path : m_Stack)
		{
			if (File::IsRootPath(path.c_str()))
				result = "";

			result += path;
		}

		return result;
	}

private:
	std::vector<std::string> m_Stack;
};

namespace
{
	PathStack StaticPathStack;
}

/**	Directory Scope
*******************************************************************************/
File::DirectoryScope::DirectoryScope( const char* directory )
{
	StaticPathStack.PushPath(directory);
	Debug_Log("Current Directory: \"%s\"", GetCurrentDirectory().c_str());
}

File::DirectoryScope::~DirectoryScope()
{
	StaticPathStack.PopPath();
	Debug_Log("Current Directory: \"%s\"", GetCurrentDirectory().c_str());
}

/**	Get Current Directory
*******************************************************************************/
std::string File::GetCurrentDirectory()
{
	return StaticPathStack.ToString();
}

/**	Is Directory
*******************************************************************************/
bool File::IsDirectory( const char* path )
{
	return strstr(path, ".") == nullptr;
}

/**	Is Root Path
*******************************************************************************/
bool File::IsRootPath( const char* path )
{
	return strstr(path, ":/") != nullptr || 
		strstr(path, ":\\") != nullptr;
}

/**	Clean Path
*******************************************************************************/
std::string File::CleanPath( const char* path )
{
	std::string result = path;
	bool isDirectory = true;

	for(int i=0; i < result.length(); ++i)
	{
		// Replace backslashes
		if (result[i] == '\\')
			result[i] = '/';

		// Remove multiple slashes
		if (result[i] == '/')
		{
			while(i + 1 < result.length() && result[i + 1] == '/' || result[i + 1] == '\\')
			{
				result.erase(i + 1, 1);
			}

			isDirectory = true;
		}

		if (result[i] == '.')
		{
			isDirectory = false;
		}
	}

	// Remove leading slashes
	while(!result.empty() && result[0] == '/')
	{
		result.erase(0, 1);
	}

	// Add trailing slash (for directories)
	if (!result.empty() && isDirectory && result[result.length() - 1] != '/')
	{
		result += "/";
	}

	return result;
}

/**	Remove File From Path
*******************************************************************************/
std::string File::RemoveFileFromPath( const char* path )
{
	bool endsWithFile = false;

	int lastSlash = 0;
	for(int i=0; i<strlen(path); ++i)
	{
		if (path[i] == '\\' || path[i] == '/')
		{
			lastSlash = i;
			endsWithFile = false;
		}

		if (path[i] == '.')
			endsWithFile = true;
	}

	std::string result = path;

	if (endsWithFile && lastSlash + 1 < result.length())
		result.erase(lastSlash + 1, result.length() - lastSlash);

	return result;
}

/**	Read File
*******************************************************************************/
bool File::ReadFile(const char* filePath, string& outString)
{
	// Push the filePath to directory stack
	std::string fileName = GetFileFromPath(filePath);

	DirectoryScope scope(filePath);
	std::string fullPath = GetCurrentDirectory() + fileName;

	// Open file
	static char buffer[10000];
	ifstream fileStream(fullPath.c_str(), ifstream::binary);
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

/**	Get File Exists
*******************************************************************************/
bool File::GetFileExists(const std::string& path)
{
	FILE* file = fopen(path.c_str(), "r");
	if (file == nullptr)
		return false;

	fclose(file);
	return true;
}


/**	********** TESTING **********
*******************************************************************************/
#define TEST_B(expr) Debug_Log("%s = %s", #expr, expr ? "true" : "false")
#define TEST_S(expr) Debug_Log("%s = %s", #expr, expr.c_str())
#define PUSH_TEST(path, stack) do { std::string prev = stack.ToString(); stack.PushPath(path); Debug_Log("%s + %s => %s", prev.c_str(), path, stack.ToString().c_str()); } while(0)
#define POP_TEST(stack) do { std::string prev = stack.ToString(); stack.PopPath(); Debug_Log("POP(%s) => %s", prev.c_str(), stack.ToString().c_str()); } while(0)

void File::_TestFile()
{
	TEST_B(IsDirectory("Hej/This\\Is\\A/Dir"));
	TEST_B(IsDirectory("Hej/This\\Is\\A/Dir/file.exe"));
	TEST_B(IsDirectory("Hej/This\\Is\\A/Dir/.git/hello"));

	TEST_S(CleanPath("Hej/This\\Is/Path"));
	TEST_S(CleanPath("/Hej//This\\Is/\\Path"));
	TEST_S(CleanPath("/This/Is/Another\\Path.exe"));

	TEST_S(RemoveFileFromPath("Hej/This\\Is/Path"));
	TEST_S(RemoveFileFromPath("/Hej//This\\Is/\\Path"));
	TEST_S(RemoveFileFromPath("/This/Is/Another\\Path.exe"));
	TEST_S(RemoveFileFromPath("/This/.git/Is/Another"));

	TEST_B(IsRootPath("Hej/This/Is/Not/Root/poop.exe"));
	TEST_B(IsRootPath("C:/Hej/This/Is/Root/poop.exe"));
	TEST_B(IsRootPath("C:\\Hej/This/Is/Root/poop.exe"));

	PathStack stack;
	PUSH_TEST("Hello", stack);
	PUSH_TEST("My/Path", stack);
	PUSH_TEST("C:/Absolute/Path", stack);
	PUSH_TEST("/../Other", stack);
	POP_TEST(stack);
	POP_TEST(stack);
	POP_TEST(stack);

	std::cin.get();
}
/*******************************************************************************/
