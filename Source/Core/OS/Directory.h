#pragma once 
#undef GetCurrentDirectory

enum class EDirectoryType
{
	Relative,
	SystemRoot,
	LocalRoot
};

/* Class that defines a directory, and handles cleaning of string, directory appending etc. */
class FDirectory
{
public:
	/* Static stuff */
	static const std::string CleanPath(const char* path);
	static const std::string RemoveFile(const char* path, std::string* outFile = nullptr);

	static FDirectory GetWorkingDirectory();

	// Local directory (for local-root lookups)
	static void SetLocalDirectory(FDirectory localDirectory);
	static const FDirectory& GetLocalDirectory();

	// Directory pushing
	static const FDirectory GetCurrentDirectory();
	static void PushDirectory(FDirectory dir);
	static void PopDirectory();
	
private:
	static FDirectory LocalDirectory;
	static std::vector<FDirectory> DirectoryStack;

	/* Local stuff */
public:
	FDirectory();
	FDirectory(const char* path);

	FDirectory operator+(const FDirectory& other) const;
	FDirectory operator+(const char* path) const;
	FDirectory& operator+=(const FDirectory& other);
	FDirectory& operator+=(const char* path);
	FDirectory& operator=(const char* path);

	bool operator==(const FDirectory& other) const;
	bool operator==(const char* path) const;

	// Conversion to string
	operator std::string() const { return m_Path; }
	const std::string& ToString() const { return m_Path; }

	// If the path given contained a filename, we want to be able to fetch it
	const std::string& GetExtractedFileName() const { return m_ExtractedFile; }

	EDirectoryType GetType() const;

	// Whether this directory actually exists on the hard-drive
	bool Exists() const;

private:
	std::string m_Path;
	std::string m_ExtractedFile;
};

/* Class used for scoping a directory, for example handling resource dependencies and
   recursive resource loading */
class FDirectoryScope
{
public:
	FDirectoryScope(FDirectory directory)
	{
		FDirectory::PushDirectory(directory);
	}
	~FDirectoryScope()
	{
		FDirectory::PopDirectory();
	}
};