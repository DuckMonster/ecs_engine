#pragma once
#include <sys/stat.h>
#include <sys/types.h>

#undef GetCurrentDirectory

namespace File
{
	struct DirectoryScope
	{
		DirectoryScope(const char* directory);
		~DirectoryScope();
	};

	std::string GetCurrentDirectory();

	void _TestFile();
	
	// Directory
	bool IsDirectory(const char* path);
	bool IsRootPath(const char* path);
	std::string CleanPath(const char* path);
	std::string RemoveFileFromPath(const char* path);
	std::string GetFileFromPath(const std::string& fullPath);

	bool ReadFile(const char* filePath, std::string& outString);

	inline time_t GetFileModifiedTime(const std::string& filePath)
	{
		struct stat result;
		if (stat(filePath.c_str(), &result) == 0)
		{
			return result.st_mtime;
		}
		else
		{
			// Some error happened, probably it's currently being saved, just return 0
			return 0;
		}
	}

	bool GetFileExists(const std::string& path);
}