#pragma once
#include <cstdint>
#include <iostream>

namespace PaperStocking {
	struct Random {
		int seed = 0;
		Random();
		Random(int seed);
		~Random() = default;
		float Range(float min, float max, int decimalPrecision = 4);
		int Range(int min, int max);
		uint Range(uint min, uint max);
	};
}
