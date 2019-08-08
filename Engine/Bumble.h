#pragma once

#include "Graphics.h"
#include "Rect.h"
#include "Surface.h"
#include "Colors.h"
#include "Random.h"
#include "Vec4.h"
#include <vector>
#include <array>

class Bumble
{
public:
	template<typename T>
	static void ExtractDigits(std::vector<T>& vec, const T& NUM);

	template<typename T>
	T Index(T x, T y, T w);

	static std::vector<Color> Blur(const int& WIDTH, const int& HEIGHT, const std::vector<Color>& INPUT);
	static std::vector<Color> ConvertSurfaceToColorVector(const Surface& surface);
	static Surface CreateColorBlendTexture(const RectUI& RECTANGLE, const Color& color_start, const Color& color_end);

	static Color RandomColor();
};

template<typename T>
inline void Bumble::ExtractDigits(std::vector<T>& vec, const T& NUM)
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
inline T Bumble::Index(T x, T y, T w)
{
	return T(y * w + x);
}
