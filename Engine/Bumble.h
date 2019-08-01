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
	void ExtractDigits(std::vector<T>& vec, const T& NUM);

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



namespace SOUND
{	
	const size_t ROTATE		= 0;
	const size_t LINE_ONE	= 1;
	const size_t LINE_TWO	= 2;
	const size_t LINE_THREE = 3;
	const size_t LINE_FOUR	= 4;
	const size_t NEW_TETRO	= 5;
	const size_t GAME_OVER	= 6;	
}

namespace KEY
{
	const size_t RESTART	= 0;
	const size_t ROTATE		= 1;
	const size_t PAUSE		= 2;
	const size_t LEFT		= 3;
	const size_t DOWN		= 4;
	const size_t RIGHT		= 5;
};

namespace BUTTON
{
	const size_t QUIT		= 0;
	const size_t VOLUME		= 1;
};

namespace MOVE
{
	const int LEFT		= -1;
	const int RIGHT		= 1;
	const int DOWN		= 1;
	const int ROTATE	= 1;
}
