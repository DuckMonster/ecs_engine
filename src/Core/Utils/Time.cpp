#include "CorePCH.h"
#include "Time.h"

using namespace std;
using namespace std::chrono;

FTimePoint FTime::m_Start;

//--------------------------------------------------- TIME POINT

/**	Constructor
*******************************************************************************/
FTimePoint::FTimePoint( ) :
	m_TimePoint( clock::now( ) )
{
}

/**	Copy Constructor
*******************************************************************************/
FTimePoint::FTimePoint( const FTimePoint && other ) :
	m_TimePoint( other.m_TimePoint )
{
}

/**	Elapsed
*******************************************************************************/
float FTimePoint::Elapsed( )
{
	timepoint nowTime = clock::now( );
	return (float)(duration_cast<microseconds>(nowTime - m_TimePoint).count( ) * 1.0e-6);
}

/**	Operator -
*******************************************************************************/
float FTimePoint::operator-( const FTimePoint & other )
{
	return (float)(duration_cast<microseconds>(m_TimePoint - other.m_TimePoint).count( ) * 1.0e-6);
}

/**	Operator =
*******************************************************************************/
void FTimePoint::operator=( const FTimePoint & other )
{
	m_TimePoint = other.m_TimePoint;
}