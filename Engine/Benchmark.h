#pragma once

#include <iostream>
#include <fstream>
#include <chrono>

class Benchmark
{
public:
	std::chrono::steady_clock::time_point A;
	std::chrono::steady_clock::time_point B;
public:
	Benchmark() = default;
	void SetA()
	{
		A = std::chrono::steady_clock::now();
	}
	void SetB()
	{
		B = std::chrono::steady_clock::now();
	}
	float TimeBetween()
	{
		return std::chrono::duration<float,std::micro>(B - A).count();
	}
};
