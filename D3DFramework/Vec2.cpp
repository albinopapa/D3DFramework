#include "Vec2.h"
//#include <intrin.h>

constexpr auto sqSec = .016f * .016f;
constexpr float grav = ( 9.8f * sqSec );
constexpr float pi = 3.1415926f;

float sqrt_helper( float X, float Cur, float Prev )
{
	return ( Cur == Prev ) ?
		Cur : sqrt_helper( X, 0.5f * ( Cur + X / Cur ), Cur );
}

float Sqrt( float Value )
{
	return sqrt_helper( Value, Value, 0.f );
}

float Sq( const float A )
{
	return A * A;
}

float CrossProduct( const Vec2f & V0, const Vec2f & V1 )
{
	return ( V0.x * V1.y ) - ( V0.y * V1.x );
}

float DotProduct( const Vec2f & V0, const Vec2f & V1 )
{
	return ( ( V0.x * V1.x ) + ( V0.y * V1.y ) );
}

float SqMagnitude( const Vec2f & V )
{
	return DotProduct( V, V );
}

float Magnitude( const Vec2f & V )
{
	return Sqrt( SqMagnitude( V ) );
}

float SqDistance( const Vec2f & V0, const Vec2f & V1 )
{
	return SqMagnitude( V1 - V0 );
}

float Distance( const Vec2f & V0, const Vec2f & V1 )
{
	return Magnitude( V1 - V0 );
}

float Recip( const float Val )
{
	return ( ( Val != 0.f ) ? 1.f / Val : Val );
}

float RecipSqrt( const float SqA )
{
	return Recip( Sqrt( SqA ) );
}

Vec2f Normalize( const Vec2f & V, const float SqMag )
{
	return V * RecipSqrt( SqMag );
}

Vec2f Normalize( const Vec2f & V )
{
	return Normalize( V, SqMagnitude( V ) );
}

Vec2f CalculateGravForce( const Vec2f & PosI, const Vec2f & PosJ, float MassI, float MassJ )
{
	const auto delta = PosJ - PosI;
	const auto recipDist = Recip( Magnitude( delta ) );
	const auto norm = delta * recipDist;
	const auto combinedMass = MassJ + MassI;
	const auto force = grav * recipDist * combinedMass;
	const auto accel = norm * force;
	return accel;
}

Vec2f ClampVelocity( const Vec2f & Vel, float MaxSpeed )
{
	const auto sqSpeed = SqMagnitude( Vel );
	const auto sqMaxSpeed = Sq( MaxSpeed );
	if( sqSpeed > sqMaxSpeed )
	{
		const auto norm = Normalize( Vel, sqSpeed );
		const auto correctedVel = norm * MaxSpeed;
		return correctedVel;
	}
	return Vel;
}

float Quadratic( const Vec2f & PointA, const Vec2f & PointB, float Radius )
{
	return
		( SqDistance( PointA, PointB ) * Sq( Radius ) ) -
		Sq( CrossProduct( PointA, PointB ) );
}