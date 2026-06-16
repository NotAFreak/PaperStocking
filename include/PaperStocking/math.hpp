#pragma once
#include <glm/glm.hpp>
namespace PaperStocking {
	// Multiply to convert degrees to radians
	constexpr double DEGTORAD = 0.0174532925199432957; // which is PI / 180
	// Multiply to convert radians to degrees
	constexpr double RADTODEG = 57.295779513082320876; // and this is 180 / PI

	// type aliases
	// god i fucking love stealing other people's hard work
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Quaternion = glm::quat;

	struct Math {
	public:
		static Vector2 MoveToward(Vector2 current, Vector2 destination, float amount);
		static Vector3 MoveToward(Vector3 current, Vector3 destination, float amount);
		static Vector4 MoveToward(Vector4 current, Vector4 destination, float amount);

		static float Magnitude(Vector2 vector);
		static float Magnitude(Vector3 vector);
		static float Magnitude(Vector4 vector);

		static float MagnitudeSq(Vector2 vector);
		static float MagnitudeSq(Vector3 vector);
		static float MagnitudeSq(Vector4 vector);
	};
}