#pragma once

#include <algorithm>
#include "Point2D.h"
#include "Size.h"
#include "Vec2.h"
#include <intrin.h>
#include <vector>

constexpr float pi = 3.1415926535897932384626433832795f;
constexpr float toRadian = pi / 180.f;

Point2D ToPoint2D( const Vec2f &V );

Vec2f operator+( const SizeF &Size, const Vec2f &V );
Vec2f operator+( const Vec2f &V, const SizeF &Size );

Vec2f operator-( const SizeF &Size, const Vec2f &V );
Vec2f operator-( const Vec2f &V, const SizeF &Size );

template<class T>
constexpr T clamp( T Val, T MinVal, T MaxVal )
{
	return ( Val > MinVal ) ? ( ( Val < MaxVal ) ? Val : MaxVal ) : MinVal;
}

// Physics stuff
struct Collidable
{
	enum Collideables
	{
		NIL, CIRCLE = 1, LINE = 2, RECT = 4,
		CIRCLE_CIRCLE = CIRCLE | CIRCLE,
		CIRCLE_LINE = CIRCLE | LINE,
		CIRCLE_RECT = CIRCLE | RECT
	};

};

struct CLine;
struct CRect
{
	CRect() = default;
	CRect( float Left, float Top, float Right, float Bottom );
	CRect( const Vec2f &TopLeft, const Vec2f &BotRight );

	bool IsOverlapping( const CRect &Rect )const;

	CLine GetTopLine()const;
	CLine GetBottomLine()const;
	CLine GetLeftLine()const;
	CLine GetRightLine()const;

	float left, top, right, bottom;
};
struct CCircle
{
	CCircle() = default;
	CCircle( const Vec2f &Center, float Radius );
	CRect GetCollisionRect()const;
	Vec2f center, velocity;
	float radius;
};
struct CLine
{
	CLine() = default;
	CLine( float StartX, float StartY, float EndX, float EndY );
	CLine( const Vec2f &Start, const Vec2f &End );

	bool CollidesWith( const CCircle &Circle, Vec2f &Point )const;
	Vec2f GetDirection()const;

	// Rotates direction (from start to end) counter clockwise 90 degrees
	Vec2f GetNormal()const;
	CRect GetCollisionRect()const;


	Vec2f start, end, direction;
};

Vec2f Reflect( const Vec2f &SurfNormal, const Vec2f &BallVelocity );

class CollisionChecker
{
public:
	enum Collideables
	{
		NIL, CIRCLE = 1, LINE = 2, RECT = 4,
		CIRCLE_CIRCLE	= CIRCLE | CIRCLE,
		CIRCLE_LINE		= CIRCLE | LINE,
		CIRCLE_RECT		= CIRCLE | RECT
	};
	class Data
	{
	public:
		Data() = default;
		Data( CCircle *A, CCircle *B, const Vec2f &Normal, const Vec2f &Point )
			:
			a( std::move( A ) ), b( std::move( B ) ),
			cObjectFlags( CIRCLE_CIRCLE ),
			point( Point ),
			normal( Normal )
		{}
		Data( CCircle *A, CLine *B, const Vec2f &Normal, const Vec2f &Point  )
			:
			a( std::move( A ) ), l( std::move( B ) ),
			cObjectFlags( CIRCLE_LINE ),
			point( Point ),
			normal( Normal )
		{}
		Data( CCircle *A, CRect *B, const Vec2f &Normal, const Vec2f &Point  )
			:
			a( std::move( A ) ), r( std::move( B ) ),
			cObjectFlags( CIRCLE_RECT ),
			point( Point ),
			normal( Normal )
		{}

	public:
		CCircle *a, *b;
		CLine *l;
		CRect *r;
		Vec2f point, normal;

	private:
		friend class CollisionSolver;
		Collideables cObjectFlags;

	};

	CollisionChecker() = default;

	bool Overlaps( CCircle &A, CCircle & B );
	bool Overlaps( CCircle &A, CLine &B );
	bool Overlaps( CCircle &A, CRect &B );

	std::vector<Data> &GetCollisions();
private:
	std::vector<Data> m_collisions;
};

class CollisionSolver
{
public:
	CollisionSolver() = default;

	CollisionChecker::Data Solve( std::vector<CollisionChecker::Data> &Collisions );

private:
	void Solve_CvC( CollisionChecker::Data &CData );
	void Solve_CvL( CollisionChecker::Data &CData );
	void Solve_CvR( CollisionChecker::Data &CData );
};

