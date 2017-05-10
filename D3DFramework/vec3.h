#pragma once

#include "Vec2.h"

class Vec3
{
public:
	Vec3() = default;
	~Vec3() = default;
	Vec3( const Vec2f &V, float Z = 1.f );
	Vec3( float X, float Y, float Z );
	
	Vec3 operator+( const Vec3 &V )const;
	Vec3 operator-()const;
	Vec3 operator-( const Vec3 &V )const;
	Vec3 operator*( const float S )const;
	Vec3 operator/( const float S )const;

	float DotProduct( const Vec3 &V )const;
	Vec3 CrossProduct( const Vec3 &V )const;
	Vec3 Normalize()const;
public:
	float x, y, z;
};

