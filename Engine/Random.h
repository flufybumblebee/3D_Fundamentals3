#pragma once

#include <random>

namespace rnd
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
}
