#pragma once
#include <iostream>
#include <chrono>

namespace LGE
{
	inline float rand(float start, float end)
	{
		return (start + static_cast<float> (std::rand()) / (static_cast <float> (RAND_MAX / (end - start))));
	}

	struct Timer
	{
		std::chrono::steady_clock::time_point start;
		std::chrono::duration<double> duration;

		Timer()
			: duration(0)
		{
			start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			duration = std::chrono::high_resolution_clock::now() - start;
			// std::cout << "\nTook: " << duration.count() * 1000.0 << "ms\n";
		}

		double now()
		{
			duration = std::chrono::high_resolution_clock::now() - start;
			return duration.count();
		}

		double nowMs()
		{
			duration = std::chrono::high_resolution_clock::now() - start;
			return duration.count() * 1000.0;
		}

		void reset()
		{
			start = std::chrono::high_resolution_clock::now();
		}

	};

}