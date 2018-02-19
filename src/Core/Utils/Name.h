#pragma once

class FName
{
	static uint s_LastId;
	static std::map<std::string, uint> s_NameMap;

public:
	FName();
	FName( const std::string& str );
	FName( const char* str ) : FName( std::string( str ) ) {}
	FName( const FName& other );

	FName& operator=( const FName& other ) { m_Id = other.m_Id; m_String = other.m_String; return *this; }
	FName& operator=( const std::string& str );
	FName& operator=( const char* str ) { return ( *this = std::string( str ) ); }
	bool operator ==( const FName& other ) const { return IsValid() && m_Id == other.m_Id; }
	bool operator ==( const std::string& str ) const { return m_String == str; }
	bool operator !=( const FName& other ) const { return !( *this == other ); }
	bool operator !=( const std::string& str ) const { return !( *this == str ); }

	bool IsValid() const { return m_Id != 0; }

	const std::string& str() const { return m_String; }
	const char* c_str() const { return m_String.c_str(); }

private:
	uint m_Id = 0;
	std::string m_String = "";
};