#pragma once

#include <iostream>
#include <fstream>
#include <chrono>

class Benchmark
{
public:
	Benchmark() = default;
	void Bench(void(*pFunc)())
	{
		std::ofstream file;
		file.open("text.txt"); // creates file and names it and opens for writing to

		const unsigned int number = 100;

		for (int i = 0; i < number; i++)
		{
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

			pFunc();

			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

			file << i << " " << duration.count() << " microseconds" << std::endl;
		}
	}
};
