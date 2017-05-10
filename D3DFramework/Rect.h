#pragma once

#include "Utilities.h"

template<class T>
class Rect_t
{
public:
	constexpr explicit Rect_t() = default;
	constexpr explicit Rect_t( T Left, T Top, T Right, T Bottom )
		:
		left( Left ), top( Top ), right( Right ), bottom( Bottom )
	{}
	Vec2_t<T> GetCenter()const
	{
		const Vec2_t<T> leftTop( left, top );
		const Vec2_t<T> rightBot( right, bottom );
		return leftTop + ( ( rightBot - leftTop ) / T( 2 ) );
	}
	constexpr T GetWidth()const
	{
		return right - left;
	}
	constexpr T GetHeight()const
	{
		return bottom - top;
	}
	
	

public:
	T left = T( 0 ), top = T( 0 ), right = T( 0 ), bottom = T( 0 );
};

using Recti = Rect_t<int>;
using Rectf = Rect_t<float>;
using Rectl = Rect_t<long>;