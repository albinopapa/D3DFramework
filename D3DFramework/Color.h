#pragma once

#if defined(min)||defined(max)
#undef min
#undef max
#endif

#include <algorithm>

class Color
{
public:
	constexpr Color() = default;
	constexpr Color( unsigned R, unsigned G, unsigned B );
	constexpr Color( Color C, unsigned A );
	constexpr Color( unsigned A, unsigned R, unsigned G, unsigned B );

	constexpr unsigned GetA() const;
	constexpr unsigned GetR() const;
	constexpr unsigned GetG() const;
	constexpr unsigned GetB() const;

	void SetA( unsigned Val );
	void SetR( unsigned Val );
	void SetG( unsigned Val );
	void SetB( unsigned Val );

	constexpr Color operator+( Color C ) const;
	constexpr Color operator-( Color C ) const;
	constexpr Color operator*( unsigned char S ) const;
	Color operator*( Color C )const;
	constexpr Color operator/( unsigned char S ) const;
	Color operator*( float S )const;
	Color operator/( float S )const;


	Color &operator+=( Color C );
	Color &operator-=( Color C );
	Color &operator*=( unsigned char S );
	Color &operator/=( unsigned char S );

private:
	unsigned value = 0u;
};

bool operator==( Color A, Color B );
bool operator!=( Color A, Color B );

namespace ColorsI
{
	constexpr Color AlphaBlend( Color A, Color B );
	constexpr Color Average( Color A, Color B );
	Color Blend( Color A, Color B, float Weight );
	void AdjustBrightness( Color &C, unsigned char BritnessValue );
	constexpr unsigned char Saturate( unsigned Val );
	
}

///////////////////////////////////// DirectX Compatible Color ////////////////////////////////
class ColorF
{
public:
	ColorF();
	ColorF( float R, float G, float B );
	ColorF( const ColorF & C, float A );
	ColorF( float A, float R, float G, float B );

	float GetA()const;
	float GetR()const;
	float GetG()const;
	float GetB()const;

	void SetA( float Val );
	void SetR( float Val );
	void SetG( float Val );
	void SetB( float Val );

	ColorF operator+( const ColorF & C )const;
	ColorF operator-( const ColorF & C )const;
	ColorF operator*( float S )const;
	ColorF operator/( float S )const;

	ColorF &operator+=( const ColorF & C );
	ColorF &operator-=( const ColorF & C );
	ColorF &operator*=( float S );
	ColorF &operator/=( float S );


private:
	float a, r, g, b;
};

bool operator==( const ColorF & A, const ColorF & B );
bool operator!=( const ColorF & A, const ColorF & B );

namespace ColorsF
{
	ColorF AlphaBlend( const ColorF & A, const ColorF & B );
	ColorF Average( const ColorF & A, const ColorF & B );
	ColorF Blend( const ColorF &A, const ColorF &B, float Weight );
	float Saturate( float Val );
	ColorF Saturate( const ColorF &Val );
	Color ToColor( const ColorF &Val );
	void AdjustBrightness( ColorF &C, float BritnessValue );
}