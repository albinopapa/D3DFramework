#include "Color.h"

constexpr Color::Color( unsigned R, unsigned G, unsigned B )
	:
	Color( 255u, R, G, B )
{}

constexpr Color::Color( Color C, unsigned A )
	:
	Color( A, C.GetR(), C.GetG(), C.GetB() )
{}

constexpr Color::Color( unsigned A, unsigned R, unsigned G, unsigned B )
	:
	value( ( A << 24ui8 ) | ( R << 16ui8 ) | ( G << 8ui8 ) | B )
{}

constexpr unsigned Color::GetA() const
{
	return ( value >> 24ui8 ) & 0xFFui8;
}

constexpr unsigned Color::GetR() const
{
	return ( value >> 16ui8 ) & 0xFFui8;
}

constexpr unsigned Color::GetG() const
{
	return ( value >> 8ui8 ) & 0xFFui8;
}

constexpr unsigned Color::GetB() const
{
	return value & 0xFFui8;
}

void Color::SetA( unsigned Val )
{
	*this = Color( Val, GetR(), GetG(), GetB() );
}

void Color::SetR( unsigned Val )
{
	*this = Color( GetA(), Val, GetG(), GetB() );
}

void Color::SetG( unsigned Val )
{
	*this = Color( GetA(), GetR(), Val, GetB() );
}

void Color::SetB( unsigned Val )
{
	*this = Color( GetA(), GetR(), GetG(), Val );
}

constexpr Color Color::operator+( Color C ) const
{
	return Color(
		GetR() + C.GetR(),
		GetG() + C.GetG(),
		GetB() + C.GetB()
	);
}

constexpr Color Color::operator-( Color C ) const
{
	return Color(
		GetR() - C.GetR(),
		GetG() - C.GetG(),
		GetB() - C.GetB()
	);
}

constexpr Color Color::operator*( unsigned char S ) const
{
	return Color(
		( GetR() * S ) >> 8ui8,
		( GetG() * S ) >> 8ui8,
		( GetB() * S ) >> 8ui8
	);
}

Color Color::operator*( Color C )const
{
	return Color(
		( GetR() * C.GetR() ) >> 8ui8,
		( GetG() * C.GetG() ) >> 8ui8,
		( GetB() * C.GetB() ) >> 8ui8
	);
}

constexpr Color Color::operator/( unsigned char S ) const
{
	return Color(
		( GetR() << 8ui8 ) / S,
		( GetG() << 8ui8 ) / S,
		( GetB() << 8ui8 ) / S
	);
}

Color Color::operator*( float S )const
{
	const auto s = static_cast< unsigned char >( S * 255.f );
	Color c = *this * s;
	return c;
}

Color Color::operator/( float S )const
{	
	const auto recScalar = static_cast<unsigned char>( ( S != 0.f ? 1.f / S : S ) * 255.f );

	Color c = *this * recScalar;

	return c;
}

Color & Color::operator+=( Color C )
{
	return *this = *this + C;
}

Color & Color::operator-=( Color C )
{
	return *this = *this - C;
}

Color & Color::operator*=( unsigned char S )
{
	return *this = *this * S;
}

Color & Color::operator/=( unsigned char S )
{
	return *this = *this / S;
}

ColorF::ColorF()
	:
	a( 0.f ), r( 0.f ), g( 0.f ), b( 0.f )
{}

ColorF::ColorF( float R, float G, float B )
	:
	ColorF( 1.f, R, G, B )
{}

ColorF::ColorF( const ColorF & C, float A )
	:
	ColorF( A, C.GetR(), C.GetB(), C.GetB() )
{}

ColorF::ColorF( float A, float R, float G, float B )
	:
	a( A ), r( R ), g( G ), b( B )
{}

float ColorF::GetA() const
{
	return a;
}

float ColorF::GetR() const
{
	return r;
}

float ColorF::GetG() const
{
	return g;
}

float ColorF::GetB() const
{
	return b;
}

void ColorF::SetA( float Val )
{
	a = Val;
}

void ColorF::SetR( float Val )
{
	r = Val;
}

void ColorF::SetG( float Val )
{
	g = Val;
}

void ColorF::SetB( float Val )
{
	b = Val;
}

ColorF ColorF::operator+( const ColorF & C ) const
{
	return ColorF(
		r + C.GetR(),
		g + C.GetG(),
		b + C.GetB()
	);
}

ColorF ColorF::operator-( const ColorF & C ) const
{
	return ColorF(
		r - C.GetR(),
		g - C.GetG(),
		b - C.GetB() );
}

ColorF ColorF::operator*( float S ) const
{
	return ColorF( 
		S * r,
		S * g,
		S * b );
}

ColorF ColorF::operator/( float S ) const
{
	return *this * ( ( S != 0.f ) ? 1.f / S : 1.f );
}

ColorF & ColorF::operator+=( const ColorF & C )
{
	return *this = *this + C;
}

ColorF & ColorF::operator-=( const ColorF & C )
{
	return *this = *this - C;
}

ColorF & ColorF::operator*=( float S )
{
	return *this = *this * S;
}

ColorF & ColorF::operator/=( float S )
{
	return *this = *this / S;
}

bool operator==( Color A, Color B )
{
	return (
		A.GetR() == B.GetR() &&
		A.GetG() == B.GetG() &&
		A.GetB() == B.GetB()
		);
}

bool operator!=( Color A, Color B )
{
	return !( A == B );
}

bool operator==( const ColorF & A, const ColorF & B )
{
	return
		( A.GetR() == B.GetR() ) &&
		( A.GetG() == B.GetG() ) &&
		( A.GetB() == B.GetB() );
}

bool operator!=( const ColorF & A, const ColorF & B )
{
	return !( A == B );
}

ColorF ColorsF::AlphaBlend( const ColorF & A, const ColorF & B )
{
	return ColorF( ( A * A.GetA() ) + B * ( 1.f - A.GetA() ) );
}

ColorF ColorsF::Average( const ColorF &A, const ColorF & B )
{
	return ColorF( ( A * .5f ) + ( B * .5f ) );
}

ColorF ColorsF::Saturate( const ColorF &Val )
{
	return ColorF(
		Saturate( Val.GetR() ),
		Saturate( Val.GetG() ),
		Saturate( Val.GetB() )
	);
}

Color ColorsF::ToColor( const ColorF &Val )
{
	return Color(
		static_cast< unsigned >( Val.GetA() * 255.f ),
		static_cast< unsigned >( Val.GetR() * 255.f ),
		static_cast< unsigned >( Val.GetG() * 255.f ),
		static_cast< unsigned >( Val.GetB() * 255.f ) );
}

float ColorsF::Saturate( float Val )
{
	return std::min( 1.f, std::max( Val, 0.f ) );
}

void ColorsF::AdjustBrightness( ColorF &C, float BritnessValue )
{
	C = Saturate( ColorF(
		C.GetR() + BritnessValue,
		C.GetG() + BritnessValue,
		C.GetB() + BritnessValue
	) );
}

ColorF ColorsF::Blend( const ColorF &A, const ColorF &B, float Weight )
{
	return ( A * Weight ) + ( B * ( 1.f - Weight ) );
}

constexpr Color ColorsI::AlphaBlend( Color A, Color B )
{
	return Color(
		( A * static_cast<unsigned char>( A.GetA() ) ) + 
		( B * static_cast<unsigned char>( 255ui8 - A.GetA() ) )
	);
}

constexpr Color ColorsI::Average( Color A, Color B )
{
	return Color( ( A * 128ui8 ) + ( B * 128ui8 ) );
}

Color ColorsI::Blend( Color A, Color B, float Weight )
{
	const auto iWeight = Saturate( static_cast< unsigned >( Weight * 255.f ) );

	return ( A * iWeight ) + ( B * static_cast<unsigned char>( 255 - iWeight ) );
}

void ColorsI::AdjustBrightness( Color &C, unsigned char BritnessValue )
{
	C = Color(
		Saturate( C.GetR() + BritnessValue ),
		Saturate( C.GetG() + BritnessValue ),
		Saturate( C.GetB() + BritnessValue )
	);
}

constexpr unsigned char ColorsI::Saturate( unsigned Val )
{
	return std::min<unsigned char>( Val, 255ui8 );
}
