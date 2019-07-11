#pragma once

#include "Graphics.h"
#include <vector>


class BumbleFunctions
{
public:
	template<typename T>
	static T SetSize(const T& COLS, const T& ROWS, const T& OFFSET);

	template<typename T>
	void ExtractDigits(std::vector<T>& vec, const T& NUM);

	template<typename T>
	T Index(T x, T y, T w);
};

template<typename T>
inline T BumbleFunctions::SetSize(const T& COLS, const T& ROWS, const T& OFFSET )
{
	T size = T(0);

	const T SCREEN_W	= Graphics::ScreenWidth;
	const T SCREEN_H	= Graphics::ScreenHeight;
	const T WIDTH		= SCREEN_W - OFFSET * 2u;
	const T HEIGHT		= SCREEN_H - OFFSET * 7u;

	if (HEIGHT / ROWS <= WIDTH / COLS)
	{
		size = HEIGHT / ROWS;
	}
	else
	{
		size = WIDTH / COLS;
	}

	return size;
}

template<typename T>
inline void BumbleFunctions::ExtractDigits(std::vector<T>& vec, const T& NUM)
{
	vec.clear();
	T result = NUM;

	if (result > 0)
	{
		while (result > 0u)
		{
			vec.push_back(result % 10u);
			result /= 10u;
		}
	}
	else
	{
		vec.push_back(0u);
	}
}

template<typename T>
inline T BumbleFunctions::Index(T x, T y, T w)
{
	return T( y * w + x );
}
