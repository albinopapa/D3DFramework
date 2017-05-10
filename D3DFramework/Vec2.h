#pragma once

template<class T>
struct alignas( 16 )Vec2_t
{
	T x = 0, y = 0;

	Vec2_t() = default;
	Vec2_t( T X, T Y )
		:
		x( X ),
		y( Y )
	{}
	Vec2_t &operator+=( const Vec2_t &V )
	{
		return *this = *this + V;
	}
	Vec2_t &operator-=( const Vec2_t &V )
	{
		return *this = *this - V;
	}
	Vec2_t &operator*=( T S )
	{
		return *this = *this * S;
	}
	Vec2_t &operator/=( T S )
	{
		return *this = *this / S;
	}

	Vec2_t operator+( const Vec2_t &V )const
	{
		return{ x + V.x, y + V.y };
	}
	Vec2_t operator-()const
	{
		return{ -x, -y };
	}
	Vec2_t operator-( const Vec2_t &V )const
	{
		return{ x - V.x, y - V.y };
	}
	Vec2_t operator*( const T S )const
	{
		return{ x * S, y * S };
	}
	Vec2_t operator/( const T S )const
	{
		return{ x / S, y / S };
	}
};

using Vec2i = Vec2_t<int>;
using Vec2f = Vec2_t<float>;
using Vec2l = Vec2_t<long>;

template<class T>
Vec2_t<T> operator*( T S, const Vec2_t<T> &V )
{
	return Vec2_t<T>( V ) *= S;
}
template<class T>
Vec2_t<T> operator/( T S, const Vec2_t<T> &V )
{
	return Vec2_t<T>( V ) /= S;
}

float Sqrt( float Value );
float Sq( const float A );
float CrossProduct( const Vec2f &V0, const Vec2f &V1 );
float DotProduct( const Vec2f &V0, const Vec2f &V1 );
float SqMagnitude( const Vec2f &V );
float Magnitude( const Vec2f &V );
float SqDistance( const Vec2f &V0, const Vec2f &V1 );
float Distance( const Vec2f &V0, const Vec2f &V1 );
float Recip( const float Val );
float RecipSqrt( const float SqA );
Vec2f Normalize( const Vec2f &V, const float SqMag );
Vec2f Normalize( const Vec2f &V );
Vec2f CalculateGravForce( const Vec2f &PosI, const Vec2f &PosJ, float MassI, float MassJ );
Vec2f ClampVelocity( const Vec2f &Vel, float MaxSpeed );
float Quadratic( const Vec2f &PointA, const Vec2f &PointB, float Radius );

