#include "Utilities.h"

using float4 = __m128;
using int4 = __m128i;

Point2D ToPoint2D( const Vec2f &V )
{
	return Point2D{ int(V.x), int(V.y) };
}

Vec2f operator+( const SizeF &Size, const Vec2f &V )
{
	return Vec2f( Size.width + V.x, Size.height + V.y );
}

Vec2f operator+( const Vec2f &V, const SizeF &Size )
{
	return Vec2f( V.x + Size.width, V.y + Size.height );
}

Vec2f operator-( const SizeF &Size, const Vec2f &V )
{
	return Vec2f( Size.width - V.x, Size.height - V.y );
}

Vec2f operator-( const Vec2f &V, const SizeF &Size )
{
	return Vec2f( V.x - Size.width, V.y - Size.height );
}

Vec2f Reflect( const Vec2f &SurfNormal, const Vec2f &BallVelocity )
{
	//r = d?2( d?n^ )n^
	return BallVelocity - ( SurfNormal * ( 2.f * DotProduct( BallVelocity, SurfNormal ) ) );
}

CLine::CLine( float StartX, float StartY, float EndX, float EndY )
	:
	start( StartX, StartY ),
	end( EndX, EndY )
{}

CLine::CLine( const Vec2f & Start, const Vec2f & End )
	:
	start( Start ),
	end( End ),
	direction( Normalize( end - start ) )
{}

bool CLine::CollidesWith( const CCircle & Circle, Vec2f &Point )const
{
	const auto delta = Circle.center - start;

	const auto direction = GetDirection();
	const auto projection = DotProduct( delta, direction );
	const auto projectedPoint = direction * projection;
	Point = start + projectedPoint;

	const auto sqDist = SqDistance( Point, Circle.center );
	if( sqDist < Sq( Circle.radius ) )
	{
		return true;
	}
	Point = { 0.f, 0.f };
	return false;
}

Vec2f CLine::GetDirection() const
{
	return direction;
}

Vec2f CLine::GetNormal() const
{
	return{ direction.y, -direction.x };
}

CRect CLine::GetCollisionRect() const
{
	return CRect(
		std::min( start.x, end.x ), std::min( start.y, end.y ),
		std::max( start.x, end.x ), std::max( start.y, end.y )
	);
}

CRect::CRect( float Left, float Top, float Right, float Bottom )
	:
	left( Left ), top( Top ), right( Right ), bottom( Bottom )
{}

CRect::CRect( const Vec2f & TopLeft, const Vec2f & BotRight )
	:
	left( TopLeft.x ), top( TopLeft.y ),
	right( BotRight.x ), bottom( BotRight.y )
{}

bool CRect::IsOverlapping( const CRect & Rect ) const
{
	return (
		( left < Rect.right  && right >  Rect.left ) &&
		( top <  Rect.bottom && bottom > Rect.top ) );
}

CLine CRect::GetTopLine() const
{
	return CLine( left, top, right, top );
}

CLine CRect::GetBottomLine() const
{
	return CLine( left, bottom, right, bottom );
}

CLine CRect::GetLeftLine() const
{
	return CLine( left, top, left, bottom );
}

CLine CRect::GetRightLine() const
{
	return CLine( right, top, right, bottom );
}

bool CollisionChecker::Overlaps( CCircle & A, CCircle & B )
{
	const Vec2f abDelta = B.center - A.center;

	const float sqDist = SqMagnitude( abDelta );
	const float radSum = A.radius + B.radius;
	const float sqRad = Sq( radSum );

	const bool collides = sqDist < sqRad;
	if( !collides ) return false;

	const Vec2f normal = Normalize( abDelta, sqDist );
	const float dist = DotProduct(normal, abDelta);
	
	const float overlap = ( radSum - dist );
	
	const Vec2f point = A.center + ( normal * ( A.radius - (overlap * .5f ) ) );
	m_collisions.push_back( Data( &A, &B, normal, point ) );


	return true;
}

bool CollisionChecker::Overlaps( CCircle & A, CLine & B )
{
	// Check overlapping Rects
	CRect cRect = A.GetCollisionRect();
	CRect lRect = B.GetCollisionRect();

	if( !(
		cRect.left < lRect.right && cRect.right > lRect.left &&
		cRect.top < lRect.bottom && cRect.bottom > lRect.top ) )
	{
		return false;
	}

	const Vec2f aStartDelta = A.center - B.start;

	const Vec2f bDirection = B.GetDirection();
	const Vec2f point = B.start + ( bDirection * ( DotProduct( bDirection, aStartDelta ) ) );
	const Vec2f normal = { bDirection.y, -bDirection.x };

	m_collisions.push_back( Data( &A, &B, normal, point ) );
	return true;
}

bool CollisionChecker::Overlaps( CCircle & A, CRect & B )
{
	if( Overlaps( A, B.GetTopLine() ) || Overlaps( A, B.GetBottomLine() ) ||
		Overlaps( A, B.GetRightLine() ) || Overlaps( A, B.GetLeftLine() ) )
	{
		return true;
	}

	return false;
}

std::vector<CollisionChecker::Data>& CollisionChecker::GetCollisions()
{
	return m_collisions;
}

CCircle::CCircle( const Vec2f & Center, float Radius )
	:
	center( Center ),
	radius( Radius )
{}

CRect CCircle::GetCollisionRect() const
{	
	return CRect(
		center - Vec2f{ radius, radius },
		center + Vec2f{ radius, radius }
	);
}

CollisionChecker::Data CollisionSolver::Solve( std::vector<CollisionChecker::Data> &Collisions )
{	
	CollisionChecker::Data data;
	for( auto &collision : Collisions )
	{
		switch( collision.cObjectFlags )
		{
			case CollisionChecker::CIRCLE_CIRCLE:
				Solve_CvC( collision );
				data = collision;
				break;
			case CollisionChecker::CIRCLE_LINE:
				Solve_CvL( collision );
				data = collision;
				break;
			case CollisionChecker::CIRCLE_RECT:
				Solve_CvR( collision );
				data = collision;
				break;
		}
	}

	return data;
}

void CollisionSolver::Solve_CvC( CollisionChecker::Data &cData )
{
	const float radsum = cData.a->radius + cData.b->radius;
	const float overlap = radsum - DotProduct( cData.normal, cData.b->center - cData.a->center );
	cData.b->center = cData.point + ( cData.normal *  cData.b->radius );
	cData.a->center = cData.point + ( -cData.normal * cData.a->radius );

	cData.a->velocity = Reflect( -cData.normal, cData.a->velocity );
	cData.b->velocity = Reflect(  cData.normal, cData.b->velocity );

	int a = 0;
}

void CollisionSolver::Solve_CvL( CollisionChecker::Data &cData )
{
	// Data members to use cData.a, cData.l

}

void CollisionSolver::Solve_CvR( CollisionChecker::Data &CData )
{}
