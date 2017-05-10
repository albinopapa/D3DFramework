#pragma once

#include <random>

class Random
{
public:	
	static int GetRandomInt(int MinVal, int MaxVal);
	static int GetRandomIntInRange( int StartValue, int Range );

	static float GetRandomFloat( float MinVal, float MaxVal );
	static float GetRandomFloatInRange( float StartValue, float Range );

private:
	static std::mt19937 rng;
};

