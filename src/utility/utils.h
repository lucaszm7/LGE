#pragma once
#include <iostream>

namespace LGE
{
	inline float rand(float start, float end)
	{
		return (start + static_cast<float> (std::rand()) / (static_cast <float> (RAND_MAX / (end - start))));
	}
}