#pragma once 

class FDirectory
{
	const std::string CleanPath(const char* path);

public:
	FDirectory();
	FDirectory(const char* path);

	FDirectory operator+(const FDirectory& other);
	FDirectory operator+(FDirectory&& other);
	FDirectory operator+(const char* path);

	FDirectory& operator=(const char* path);

	const std::string& ToString() const { return m_Path; }

	bool IsRoot() const;
	bool Exists() const;

private:
	std::string m_Path;
};