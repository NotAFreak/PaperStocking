#pragma once
#include <random>
#include <cstdint>
#include <iostream>

namespace PaperStocking {
	struct Random {
		std::mt19937 seed;
		Random() : seed(std::random_device{}()) {};
		Random(int seed);
		~Random() = default;
		float Range(float min, float max);
		int Range(int min, int max);
	};
}
