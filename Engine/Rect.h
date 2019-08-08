/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	TileRect.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "Vec2.h"
#include <algorithm>
#include <memory>

template < typename T >
class _Rect
{
public:
	inline	_Rect() {}
	inline	_Rect( T top,T bottom,T left,T right )
		:
		top(	top ),
		bottom( bottom ),
		left(	left ),
		right(	right )
	{}
	inline	_Rect( const _Rect& rect )
		:
		top(	rect.top ),
		bottom( rect.bottom ),
		left(	rect.left ),
		right(	rect.right )
	{}
	inline _Rect( _Rect&& rect ) noexcept
		:
		top(	std::move(rect.top)),
		bottom(	std::move(rect.bottom)),
		left(	std::move(rect.left)),
		right(	std::move(rect.right))
	{}
	inline _Rect<T>& operator = (const _Rect<T>& rhs)
	{
		top		= rhs.top;
		bottom	= rhs.bottom;
		left	= rhs.left;
		right	= rhs.right;

		return *this;
	}
	inline _Rect<T>& operator + (const _Rect<T>& rhs)
	{
		top += rhs.top;
		bottom += rhs.bottom;
		left += rhs.left;
		right += rhs.right;

		return *this;
	}
	inline	_Rect( _Vec2<T> p0,_Vec2<T> p1 )
		:
		_Rect( min( p0.y,p1.y ),
			max( p0.y,p1.y ),
			min( p0.x,p1.x ),
			max( p0.x,p1.x ) )
	{}
	inline	void Translate( _Vec2<T> d )
	{
		Translate( d.x,d.y );
	}
	inline	void Translate( T dx,T dy )
	{
		top += dy;
		bottom += dy;
		left += dx;
		right += dx;
	}
	template <typename T2>
	inline	operator _Rect<T2>() const
	{
		return { (T2)top,(T2)bottom,(T2)left,(T2)right };
	}
	
	inline	void ClipTo( const _Rect& rect )
	{
		top		= std::max( top,	rect.top );
		bottom	= std::min( bottom,	rect.bottom );
		left	= std::max( left,	rect.left );
		right	= std::min( right,	rect.right );
	}
	inline	T GetWidth() const
	{
		return right - left;
	}
	inline	T GetHeight() const
	{
		return bottom - top;
	}
	inline	bool Overlaps( const _Rect& rect ) const
	{
		return	top		< rect.bottom	&&
				bottom	> rect.top		&& 
				left	< rect.right	&&
				right	> rect.left;
	}
	template <typename T2>
	inline	bool Contains( _Vec2<T2> p ) const
	{
		return p.y >= (T2)top && p.y <= (T2)bottom && p.x >= (T2)left && p.x <= (T2)right;
	}
	template <typename T2>
	inline	bool Contains( _Rect<T2> p ) const
	{
		return p.top >= (T2)top && p.bottom <= (T2)bottom && p.left >= (T2)left && p.right <= (T2)right;
	}
public:
	T top		= (T)0;
	T bottom	= (T)0;
	T left		= (T)0;
	T right		= (T)0;
};

//typedef _Rect< unsigned int > RectUI;
typedef _Rect< int > RectI;
typedef _Rect< float > RectF;

using RectUI = _Rect< unsigned int >;