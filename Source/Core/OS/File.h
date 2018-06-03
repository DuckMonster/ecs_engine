#pragma once
class FFile
{
public:
	static std::string GetFileNameFromPath(const char* path);

public:
	FFile();
	FFile(const char* path);
	FFile(const std::string& path);

	// Check if the file actually exists on the system
	bool Exists() const;
	bool ReadAll(std::string& outContent) const;

	time_t GetModifiedTime() const;
	std::string GetFileName() const;
	const std::string& GetFilePath() const { return m_Path; }

	// Operators
	bool operator==(const char* path) const;
	bool operator==(const std::string& path) const { return *this == path.c_str(); }
	FFile& operator=(const char* path);
	FFile& operator=(const std::string& path) { return (*this = path.c_str()); }

private:
	std::string m_Path;
};