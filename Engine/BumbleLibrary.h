#pragma once

#include <random>
#include <vector>
#include "Colors.h"
#include "Surface.h"

namespace Bumble
{
	template <typename T>
	T RandomInt(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<T> dist(min, max);
		T result = dist(rng);
		return result;
	}

	template <typename T>
	T RandomFloat(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_real_distribution<T> dist(min, max);
		T result = dist(rng);
		return result;
	}

	std::vector<Color>	ConvertSurfaceToColorVector(const Surface& surface)
	{
		std::vector<Color> output;

		for (size_t iy = 0; iy < surface.GetHeight(); iy++)
		{
			for (size_t ix = 0; ix < surface.GetWidth(); ix++)
			{
				unsigned int x = static_cast<unsigned int>(ix);
				unsigned int y = static_cast<unsigned int>(iy);
				output.push_back(surface.GetPixel(x, y));
			}
		}

		return std::move(output);
	}
	Surface				ConvertColorVectorToSurface(int w, int h, const std::vector<Color>& colors)
	{
		Surface surface(w, h);

		for (size_t iy = 0; iy < h; iy++)
		{
			for (size_t ix = 0; ix < w; ix++)
			{
				size_t i = iy * static_cast<size_t>(w) + ix;
				surface.PutPixel(static_cast<unsigned int>(ix), static_cast<unsigned int>(iy), colors[i]);
			}
		}

		return std::move(surface);
	}
}

