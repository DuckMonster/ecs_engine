#include "CorePCH.h"
#include "Path.h"
#include "StringUtils.h"

using namespace std;

/**	Push Name
*******************************************************************************/
void FPath::PushName( const FName& name )
{
	m_PathNames.push_back( name );
}

/**	Pop Name
*******************************************************************************/
FName FPath::PopName( )
{
	if (m_PathNames.empty())
		return FName();

	FName topName = m_PathNames[m_PathNames.size() - 1];
	m_PathNames.erase( m_PathNames.begin() + m_PathNames.size() - 1 );

	return topName;
}

/**	Contains
*******************************************************************************/
bool FPath::Contains( const FName& name ) const
{
	for ( uint32 i = 0; i < m_PathNames.size(); i++ )
	{
		if ( m_PathNames[i] == name )
			return true;
	}

	return false;
}

/**	Operator == FPath
*******************************************************************************/
bool FPath::operator==( const FPath & other ) const
{
	if ( m_PathNames.size() != other.m_PathNames.size() )
		return false;

	for ( uint32 i = 0; i < m_PathNames.size(); i++ )
	{
		if ( m_PathNames[i] != other.m_PathNames[i] )
			return false;
	}

	return true;
}

/**	Operators
*******************************************************************************/
FPath FPath::operator+( const FPath& other ) const
{
	FPath newPath(*this);
	newPath += other;
	
	return newPath;
}

FPath FPath::operator+( const FName & name ) const
{
	FPath newPath( *this );
	newPath += name;

	return newPath;
}

FPath& FPath::operator+=( const FPath& other )
{
	for ( const FName& path : other.m_PathNames )
		m_PathNames.push_back( path );

	return *this;
}

FPath& FPath::operator+=( const FName& name )
{
	m_PathNames.push_back( name );
	return *this;
}

/**	To URI
*******************************************************************************/
string FPath::ToURI() const
{
	if ( m_PathNames.size() == 0 )
		return string( "/" );

	string result;

	for ( uint32 i = 0; i < m_PathNames.size(); i++ )
	{
		result += "/" + m_PathNames[i].str();
	}

	return result;
}

/**	Get Name
*******************************************************************************/
const FName FPath::GetName( uint32 index ) const
{
	if ( !Ensure( index < m_PathNames.size() ) )
		return FName();

	return m_PathNames[index];
}

/**	From URI
*******************************************************************************/
void FPath::FromURI( const string& str )
{
	Clear();

	vector<string> tokenized;
	StringUtils::Tokenize( str, '/', tokenized, true );

	for ( const string& str : tokenized )
		m_PathNames.emplace_back( str );
}
