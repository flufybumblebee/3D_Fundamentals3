#pragma once

#include <random>

namespace RND
{
	template <typename T>
	T Random(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<T> dist(min, max);
		T result = dist(rng);
		return result;
	}
}

