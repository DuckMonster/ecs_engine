#include "CorePCH.h"
#include "Random.h"

#ifdef max
#undef max
#undef min
#endif

std::random_device CRandom::s_Device;

/**	Value
*******************************************************************************/
float CRandom::Value()
{
	static std::uniform_real_distribution<float> DIST( 0.f, 1.f );
	return DIST( s_Device );
}

/**	Value
*******************************************************************************/
float CRandom::Value( float max )
{
	std::uniform_real_distribution<float> distribution( 0.f, max );
	return distribution( s_Device );
}

/**	Value
*******************************************************************************/
float CRandom::Value( float min, float max )
{
	Ensure( min < max );

	std::uniform_real_distribution<float> distribution( min, max );
	return distribution( s_Device );
}

/**	Bool
*******************************************************************************/
bool CRandom::Bool()
{
	return Value() >= 0.5f;
}

/**	Chance
*******************************************************************************/
bool CRandom::Chance( float chance )
{
	return Value() > chance;
}
