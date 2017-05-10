#include "Random.h"

std::mt19937 Random::rng = std::mt19937( std::seed_seq() );

int Random::GetRandomInt( int MinVal, int MaxVal )
{
	std::uniform_int_distribution<int> distrobution( MinVal, MaxVal );
	return distrobution( rng );
}

int Random::GetRandomIntInRange( int StartValue, int Range )
{
	return GetRandomInt( StartValue, ( StartValue + Range ) - 1 );
}

float Random::GetRandomFloat( float MinVal, float MaxVal )
{
	std::uniform_real_distribution<float> distrobution( MinVal, MaxVal );
	return distrobution( rng );
}

float Random::GetRandomFloatInRange( float StartValue, float Range )
{
	return GetRandomFloat( StartValue, ( StartValue + Range ) - 1.f );
}
