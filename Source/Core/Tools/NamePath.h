#pragma once
#include "Name.h"

class FNamePath
{
public:
	FNamePath() {}
	FNamePath( const FNamePath& other ) : m_PathNames( other.m_PathNames ) {}
	FNamePath( const std::string& str ) { FromURI( str ); }

	void PushName( const FName& name );
	FName PopName();
	bool Contains( const FName& name ) const;

	FNamePath& operator =( const FNamePath& other ) { m_PathNames = other.m_PathNames; return *this; }
	FNamePath& operator =( const std::string& uri ) { FromURI( uri ); return *this; }
	bool operator == ( const FNamePath& other ) const;
	bool operator == ( const std::string& uri ) const { return ( *this == FNamePath( uri ) ); }
	FNamePath operator +( const FNamePath& other ) const;
	FNamePath operator +( const FName& name ) const;
	FNamePath& operator +=( const FNamePath& other );
	FNamePath& operator += ( const FName& name );

	const FName operator[]( uint32 index ) const { return GetName( index ); }

	size_t Length() const { return m_PathNames.size(); }
	void Clear() { m_PathNames.clear(); }

	std::string ToURI() const;
	const FName GetName( uint32 index ) const;

private:
	void FromURI( const std::string& str );

	std::vector<FName> m_PathNames;
};