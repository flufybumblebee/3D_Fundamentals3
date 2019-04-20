#pragma once

namespace rnd
{
	template <typename T>
	T RandomInt(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<T> dist(min, max);

		return dist(rng);
	}

	template <typename T>
	T RandomFloat(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_real_distribution<T> dist(min, max);

		return dist(rng);
	}
};
