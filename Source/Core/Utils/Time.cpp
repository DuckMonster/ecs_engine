#include "CorePCH.h"
#include "Time.h"

using namespace std;
using namespace std::chrono;

FTimePoint FTime::m_Start;
FTimePoint FTime::m_PreviousDeltaPoint;
float FTime::m_FrameDelta = 0.f;
bool FTime::m_FirstFrame = true;

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

/**	Update Delta
*******************************************************************************/
void FTime::UpdateDelta()
{
	FTimePoint now;
	m_FrameDelta = now - m_PreviousDeltaPoint;

	m_PreviousDeltaPoint = now;

	// We don't want a big spike on the first frame, so just skip it
	if (m_FirstFrame)
	{
		m_FrameDelta = 0.f;
		m_FirstFrame = false;
	}
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
