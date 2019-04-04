#pragma once

template <typename T>
class Random
{
private:
	T min;
	T max;
public:
	Random(T min, T max)
		:
		min(min),
		max(max)
	{}
	T operator( T min, T max )
	{
		T result;
		return result;
	}
};
