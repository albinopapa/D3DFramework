#pragma once

template<class T>
struct Size_t
{
	constexpr Size_t() = default;
	constexpr Size_t( T Width, T Height )
		:
		width( Width ),
		height( Height )
	{}

	Size_t operator+=( const Size_t &Size )
	{
		width += Size.width;
		height += Size.height;

		return *this;
	}
	constexpr Size_t operator+( const Size_t &Size )const
	{
		return Size_t( *this ) += Size;
	}

	Size_t operator-=( const Size_t &Size )
	{
		width -= Size.width;
		height -= Size.height;

		return *this;
	}
	constexpr Size_t operator-( const Size_t &Size )const
	{
		return Size_t( *this ) -= Size;
	}

	Size_t operator*=( const float S )
	{
		width *= S;
		height *= S;
		return *this;
	}
	constexpr Size_t operator*( const float S )const
	{
		return Size_t( *this ) *= S;
	}

	// Members
	T width = static_cast< T >( 0. ), height = static_cast< T >( 0. );
};

using SizeF = Size_t<float>;
using SizeI = Size_t<int>;
using SizeU = Size_t<unsigned>;