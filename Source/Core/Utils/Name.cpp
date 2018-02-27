#include "CorePCH.h"
#include "Name.h"

using namespace std;

uint32 FName::s_LastId = 1;
map<std::string, uint32> FName::s_NameMap;

/**	Constructor
*******************************************************************************/
FName::FName()
{
}

/**	Constructor
*******************************************************************************/
FName::FName( const std::string& str )
{
	*this = str;
}

/**	Constructor
*******************************************************************************/
FName::FName( const FName& other ) : m_Id( other.m_Id ), m_String( other.m_String )
{
}

/**	Operator = string
*******************************************************************************/
FName& FName::operator=( const string& str )
{
	map<string, uint32>::iterator it = s_NameMap.find( str );
	if ( it == s_NameMap.end() )
	{
		m_Id = s_LastId++;
		s_NameMap[str] = m_Id;
	}
	else
	{
		m_Id = it->second;
	}

	m_String = str;
	return *this;
}
