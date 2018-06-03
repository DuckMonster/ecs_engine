#pragma once
class FFile
{
public:
	static const char* GetFileNameFromPath(const char* path);
	static const char* GetExtensionFromPath(const char* path);

public:
	FFile();
	FFile(const char* path);
	FFile(const std::string& path);

	// Check if the file actually exists on the system
	bool Exists() const;
	bool ReadAll(std::string& outContent) const;

	time_t GetModifiedTime() const;
	const char* GetFileName() const;
	const char* GetPath() const { return m_Path.c_str(); }
	const char* GetExtension() const;

	// Operators
	bool operator==(const char* path) const;
	bool operator==(const std::string& path) const { return *this == path.c_str(); }
	bool operator==(const FFile& other) const;
	FFile& operator=(const char* path);
	FFile& operator=(const std::string& path) { return (*this = path.c_str()); }
	FFile& operator=(const FFile& other);

private:
	std::string m_Path;
};