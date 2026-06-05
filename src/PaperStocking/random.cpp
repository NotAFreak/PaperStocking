#include <cmath>
#include <PaperStocking/random.hpp>

using namespace PaperStocking;

Random::Random() {
	this->seed = rand();
}

Random::Random(int seed) {
	this->seed = seed;
}

float Random::Range(float min, float max, int decimalPrecision) {
	srand(seed);
	return
		static_cast<float>(Range((int)min, (int)max))
		+ (((float)rand()) / ((float)rand()));
}

int Random::Range(int min, int max) {
	srand(seed);
	int range = max - min + 1;
	return (int)rand() % range + min;
}

uint Random::Range(uint min, uint max) {
	srand(seed);
	uint range = max - min + 1;
	return (uint)rand() % range + min;
}
