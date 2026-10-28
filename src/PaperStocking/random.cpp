#include <cmath>
#include <PaperStocking/random.hpp>

using namespace PaperStocking;

Random::Random(int seed) {
	// this->seed = seed;
}

float Random::Range(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(seed);
}

int Random::Range(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(seed);
}