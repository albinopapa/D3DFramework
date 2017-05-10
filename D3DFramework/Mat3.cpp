#include "Mat3.h"
#include <math.h>

Mat3::Mat3( const Vec3 & R0, const Vec3 & R1, const Vec3 & R2 )
	:
	r0( R0 ),
	r1( R1 ),
	r2( R2 )
{}

Mat3 Mat3::operator*( const Mat3 & M )const
{
	Mat3 m( M.Transpose() );
	return{
		{ r0.DotProduct( m.r0 ), r0.DotProduct( m.r1 ), r0.DotProduct( m.r2 ) },
		{ r1.DotProduct( m.r0 ), r1.DotProduct( m.r1 ), r1.DotProduct( m.r2 ) },
		{ r2.DotProduct( m.r0 ), r2.DotProduct( m.r1 ), r2.DotProduct( m.r2 ) }
	};
}

Vec2f Mat3::operator*( const Vec2f & V ) const
{
	Vec3 v( V );
	Mat3 m( Transpose() );	
	
	return{
		v.DotProduct( m.r0 ),
		v.DotProduct( m.r1 )
	};
}

Mat3 Mat3::Identity()
{
	return{
		{ 1.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f },
		{ 0.f, 0.f, 1.f }
	};
}

Mat3 Mat3::Transpose()const
{
	return{
		{ r0.x, r1.x, r2.x },
		{ r0.y, r1.y, r2.y },
		{ r0.z, r1.z, r2.z }
	};
}

Mat3 Mat3::Translation( const Vec2f & V )
{	
	return{
		{ 1.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f },
		{ V }
	};
}

Mat3 Mat3::Scale( const Vec2f & V )
{
	return{
		{ V.x, 0.f, 0.f },
		{ 0.f, V.y, 0.f },
		{ 0.f, 0.f, 1.f }
	};
}

Mat3 Mat3::Rotation( const float Theta )
{
	float cosine = cosf( Theta );
	float sine = sinf( Theta );
		
	return{
		{ cosine, sine, 0.f },
		{ -sine, cosine, 0.f },
		{ 0.f, 0.f, 1.f }
	};
}
