#include <glm/gtx/norm.hpp>
#include <PaperStocking/math.hpp>

using namespace PaperStocking;

Vector2 Math::MoveToward(Vector2 current, Vector2 destination, float amount) {
	// return glm::clamp(current + Vector2(amount), Vector2(0), destination);
	Vector2 a = destination - current;
	float magnitude = Magnitude(a);
	if (magnitude <= amount) {
		return destination;
	}
	return current + a / magnitude * amount;
}
Vector3 Math::MoveToward(Vector3 current, Vector3 destination, float amount) {
	Vector3 newVector = current + ((destination - current / destination - current) * amount);
	return newVector;
}
Vector4 Math::MoveToward(Vector4 current, Vector4 destination, float amount) {
	Vector4 newVector = current + ((destination - current / destination - current) * amount);
	return newVector;
}

float Math::Magnitude(Vector2 vector) {
	return glm::length(vector);
}
float Math::Magnitude(Vector3 vector) {
	return glm::length(vector);
}
float Math::Magnitude(Vector4 vector) {
	return glm::length(vector);
}

float Math::MagnitudeSq(Vector2 vector) {
	return glm::length2(vector);
}
float Math::MagnitudeSq(Vector3 vector) {
	return glm::length2(vector);
}
float Math::MagnitudeSq(Vector4 vector) {
	return glm::length2(vector);
}
