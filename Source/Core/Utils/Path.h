#pragma once
#include "Name.h"

class FPath
{
public:
	FPath() {}
	FPath( const FPath& other ) : m_PathNames( other.m_PathNames ) {}
	FPath( const std::string& str ) { FromURI( str ); }

	void PushName( const FName& name );
	FName PopName();
	bool Contains( const FName& name ) const;

	FPath& operator =( const FPath& other ) { m_PathNames = other.m_PathNames; return *this; }
	FPath& operator =( const std::string& uri ) { FromURI( uri ); return *this; }
	bool operator == ( const FPath& other ) const;
	bool operator == ( const std::string& uri ) const { return ( *this == FPath( uri ) ); }
	FPath operator +( const FPath& other ) const;
	FPath operator +( const FName& name ) const;
	FPath& operator +=( const FPath& other );
	FPath& operator += ( const FName& name );

	const FName operator[]( uint32 index ) const { return GetName( index ); }

	size_t Length() const { return m_PathNames.size(); }
	void Clear() { m_PathNames.clear(); }

	std::string ToURI() const;
	const FName GetName( uint32 index ) const;

private:
	void FromURI( const std::string& str );

	std::vector<FName> m_PathNames;
};