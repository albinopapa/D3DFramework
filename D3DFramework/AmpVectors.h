#pragma once

#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>

namespace Amp
{
	struct alignas( 16 )Vec2f
	{
		Vec2f()restrict( cpu, amp )
			:
			Vec2f( 0.f, 0.f )
		{}
		Vec2f( float X, float Y )restrict( cpu, amp )
			:
			x( X ), y( Y )
		{}

		Vec2f &operator+=( const Vec2f &V )restrict( cpu, amp )
		{
			x += V.x;
			y += V.y;
			return *this;
		}
		Vec2f &operator-=( const Vec2f &V )restrict( cpu, amp )
		{
			x -= V.x;
			y -= V.y;

			return *this;
		}
		Vec2f &operator*=( float V )restrict( cpu, amp )
		{
			x *= V;
			y *= V;

			return *this;
		}
		Vec2f &operator/=( float V )restrict( cpu, amp )
		{
			*this *= ( 1.f / V );
			return *this;
		}

		Vec2f operator+( const Vec2f &V )const restrict( cpu, amp )
		{
			return Vec2f( *this ) += V;
		}
		Vec2f operator-( const Vec2f &V )const restrict( cpu, amp )
		{
			return Vec2f( *this ) -= V;
		}
		Vec2f operator*( float V )const restrict( cpu, amp )
		{
			return Vec2f( *this ) *= V;
		}
		Vec2f operator/( float V )const restrict( cpu, amp )
		{
			return Vec2f( *this ) /= V;
		}

		float Dot( const Vec2f &V )const restrict( cpu, amp )
		{
			return ( ( x * V.x ) + ( y * V.y ) );
		}
		float Length()const restrict( amp )
		{
			using namespace Concurrency;
			return precise_math::sqrtf( Dot( *this ) );
		}
		Vec2f Normalize()const restrict( amp )
		{
			return *this * ( 1.f / Length() );
		}

		float x, y;
	};

	struct alignas( 16 )Vec3f
	{
		Vec3f()restrict( cpu, amp )
			:
			x( 0.f ),
			y( 0.f ),
			z( 0.f )
		{}
		Vec3f( float X, float Y, float Z )restrict( cpu, amp )
			:
			x( X ),
			y( Y ),
			z( Z )
		{}

		Vec3f &operator+=( const Vec3f &V )restrict( cpu, amp )
		{
			x += V.x;
			y += V.y;
			z += V.z;
			return *this;
		}
		Vec3f &operator-=( const Vec3f &V )restrict( cpu, amp )
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;

			return *this;
		}
		Vec3f &operator*=( float V )restrict( cpu, amp )
		{
			x *= V;
			y *= V;
			z *= V;

			return *this;
		}
		Vec3f &operator/=( float V )restrict( cpu, amp )
		{
			*this *= ( 1.f / V );
			return *this;
		}

		Vec3f operator+( const Vec3f &V )const restrict( cpu, amp )
		{
			return Vec3f( *this ) += V;
		}
		Vec3f operator-( const Vec3f &V )const restrict( cpu, amp )
		{
			return Vec3f( *this ) -= V;
		}
		Vec3f operator*( float V )const restrict( cpu, amp )
		{
			return Vec3f( *this ) *= V;
		}
		Vec3f operator/( float V )const restrict( cpu, amp )
		{
			return Vec3f( *this ) /= V;
		}				
		float Dot( const Vec3f &V )const restrict( cpu, amp )
		{
			return ( ( x * V.x ) + ( y * V.y ) + ( z * V.z ) );			
		}				
		float Length()const restrict( cpu, amp )
		{
			using Concurrency::precise_math::sqrtf;
			using std::sqrtf;
			return sqrtf( Dot( *this ) );
		}
		Vec3f Normalize()const restrict( cpu, amp )
		{
			return *this * ( 1.f / Length() );
		}

		float x, y, z;
	};

	struct alignas( 16 )Vec4f
	{
		Vec4f()restrict( cpu, amp )
			:
			Vec4f( 0.f, 0.f, 0.f, 0.f )
		{}
		Vec4f( float X, float Y, float Z, float W )restrict( cpu, amp )
			:
			x( X ), y( Y ), z( Z ), w( W )
		{}

		Vec4f &operator+=( const Vec4f &V )restrict( cpu, amp )
		{
			x += V.x;
			y += V.y;
			z += V.z;
			w += V.w;

			return *this;
		}
		Vec4f &operator-=( const Vec4f &V )restrict( cpu, amp )
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;
			w -= V.w;

			return *this;
		}
		Vec4f &operator*=( float V )restrict( cpu, amp )
		{
			x *= V;
			y *= V;
			z *= V;
			w *= V;

			return *this;
		}
		Vec4f &operator/=( float V )restrict( cpu, amp )
		{
			*this *= ( 1.f / V );
			return *this;
		}

		Vec4f operator+( const Vec4f &V )const restrict( cpu, amp )
		{
			return Vec4f( *this ) += V;
		}
		Vec4f operator-( const Vec4f &V )const restrict( cpu, amp )
		{
			return Vec4f( *this ) -= V;
		}
		Vec4f operator*( float V )const restrict( cpu, amp )
		{
			return Vec4f( *this ) *= V;
		}
		Vec4f operator/( float V )const restrict( cpu, amp )
		{
			return Vec4f( *this ) /= V;
		}

		float Dot( const Vec4f &V )const restrict( cpu, amp )
		{
			return (
				( x * V.x ) +
				( y * V.y ) +
				( z * V.z ) +
				( w * V.w ) );
		}
		float Length()const restrict( amp )
		{
			return Concurrency::precise_math::sqrtf( Dot( *this ) );
		}
		Vec4f Normalize()const restrict( amp )
		{
			return *this * ( 1.f / Length() );
		}

		float x, y, z, w;
	};

	struct alignas( 16 )Colorf
	{
		Colorf()restrict( cpu, amp )
			:
			Colorf( 0.f, 0.f, 0.f )
		{}
		Colorf( float A, float R, float G, float B )restrict( cpu, amp )
			:
			a( A ), r( R ), g( G ), b( B )
		{}
		Colorf( float R, float G, float B )restrict( cpu, amp )
			:
			Colorf( 1.f, R, G, B )
		{}

		Colorf &operator+=( const Colorf &V )restrict( cpu, amp )
		{
			a += V.a;
			r += V.r;
			g += V.g;
			b += V.b;
			return *this;
		}
		Colorf &operator-=( const Colorf &V )restrict( cpu, amp )
		{
			a -= V.a;
			r -= V.r;
			g -= V.g;
			b -= V.b;

			return *this;
		}
		Colorf &operator*=( float V )restrict( cpu, amp )
		{
			a *= V;
			r *= V;
			g *= V;
			b *= V;

			return *this;
		}
		Colorf &operator/=( float V )restrict( cpu, amp )
		{
			*this *= ( 1.f / V );
			return *this;
		}
		Colorf &operator*=( const Colorf &V )restrict( cpu, amp )
		{
			a *= V.a;
			r *= V.r;
			g *= V.g;
			b *= V.b;

			return *this;
		}
		Colorf &operator/=( const Colorf &V )restrict( cpu, amp )
		{
			*this *= Colorf( 1.f / V.a, 1.f / V.r, 1.f / V.g, 1.f / V.b );
			return *this;
		}

		Colorf operator+( const Colorf &V )const restrict( cpu, amp )
		{
			return Colorf( *this ) += V;
		}
		Colorf operator-( const Colorf &V )const restrict( cpu, amp )
		{
			return Colorf( *this ) -= V;
		}
		Colorf operator*( float V )const restrict( cpu, amp )
		{
			return Colorf( *this ) *= V;
		}
		Colorf operator*( const Colorf &V )const restrict( cpu, amp )
		{
			return Colorf( *this ) *= V;
		}
		Colorf operator/( float V )const restrict( cpu, amp )
		{
			return Colorf( *this ) /= V;
		}
		Colorf operator/( const Colorf &V )const restrict( cpu, amp )
		{
			return Colorf( *this ) /= V;
		}

		Colorf Saturate()restrict( amp, cpu )
		{
			float ta = a >= 0.f ? a : 0.f;
			float tr = r >= 0.f ? r : 0.f;
			float tg = g >= 0.f ? g : 0.f;
			float tb = b >= 0.f ? b : 0.f;
			ta = a <= 1.f ? a : 1.f;
			tr = r <= 1.f ? r : 1.f;
			tg = g <= 1.f ? g : 1.f;
			tb = b <= 1.f ? b : 1.f;
			return Colorf( ta, tr, tg, tb );
		}
		unsigned Pack()restrict( cpu, amp )
		{
			const Colorf temp( *this * 255.f );

			const unsigned ta = unsigned( temp.a );
			const unsigned tr = unsigned( temp.r );
			const unsigned tg = unsigned( temp.g );
			const unsigned tb = unsigned( temp.b );

			return ( ( ta << 24 ) | ( tr << 16 ) | ( tg << 8 ) | tb );
		}

		float a, r, g, b;
	};

	inline bool RaySphereIntersection(
		const Vec3f &SpherePosition, float SphereRadius,
		const Vec3f &Origin, const Vec3f &Direction, float &RayLength )restrict( amp )
	{
		using Concurrency::precise_math::sqrtf;

		const Vec3f originToSphere = SpherePosition - Origin;
		const float projection = originToSphere.Dot( Direction );
		const Vec3f distanceVector = originToSphere - Direction * projection;
		const float distanceSq = distanceVector.Dot( distanceVector );
		const float radiusSq = SphereRadius * SphereRadius;

		if( distanceSq > radiusSq )
		{
			return false;
		}

		const float newLength = projection - sqrtf( radiusSq - distanceSq );
		if( newLength < RayLength && newLength > 0.0f )
		{
			RayLength = newLength;
			return true;
		}
		return false;
	}
}
