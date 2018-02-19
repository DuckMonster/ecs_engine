#pragma once
#include <random>

class CRandom
{
public:
	static float Value();
	static float Value( float max );
	static float Value( float min, float max );

	static bool Bool();
	static bool Chance( float chance );

private:
	static std::random_device s_Device;
};