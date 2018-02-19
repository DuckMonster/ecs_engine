#pragma once
#include <chrono>

class FTimePoint
{
	typedef std::chrono::high_resolution_clock::time_point timepoint;
	typedef std::chrono::high_resolution_clock clock;

public:
	FTimePoint( );
	FTimePoint( const FTimePoint&& other );

	float Elapsed( );
	float operator-( const FTimePoint& other );
	void operator=( const FTimePoint& other );

private:
	timepoint	m_TimePoint;
};

class FTime
{
public:
	static float TotalElapsed( ) { return m_Start.Elapsed( ); }
	static void Reset( ) { m_Start = FTimePoint( ); }

private:
	static FTimePoint	m_Start;
};