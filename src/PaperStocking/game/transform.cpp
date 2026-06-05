#include <PaperStocking/game/transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using namespace PaperStocking;
Transform::Transform(glm::vec3 pos) {
	this->position = pos;
}
Transform::Transform(glm::vec3 pos, glm::quat rotation) {
	this->position = pos;
	this->rotation = rotation;
}
Transform::Transform(glm::vec3 pos, glm::vec3 scale) {
	this->position = pos;
	this->scale = scale;
}
Transform::Transform(glm::vec3 pos, glm::quat rotation, glm::vec3 scale) {
	this->position = pos;
	this->rotation = rotation;
	this->scale = scale;
}
glm::mat4 Transform::GetLocalMatrix() {
	glm::mat4 p = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 r = glm::toMat4(rotation);
	glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

	return p * r * s;
}
glm::mat4 Transform::GetMatrix() {
	glm::mat4 localMat = GetLocalMatrix();
	if(parent != nullptr) {
		return parent->GetMatrix() * localMat;
	}
	return localMat;
}
glm::vec3 Transform::GetGlobalPosition() {
	return glm::vec3{GetMatrix()[3]};
}
glm::quat Transform::GetGlobalRotation() {
	return glm::quat_cast(GetMatrix());
}
glm::vec3 Transform::GetGlobalEulerAngles() {
	return glm::eulerAngles(GetGlobalRotation());
}
void Transform::SetGlobalPosition(glm::vec3 pos) {
	if (parent) {
		const glm::mat4 parentMatrix = parent->GetMatrix();
		const glm::mat4 inverseParent = glm::inverse(parentMatrix);
		this->position = glm::vec3(inverseParent * glm::vec4(pos, 1.0f));
	} else {
		this->position = pos;
	}
}
void Transform::SetGlobalRotation(glm::quat rot) {
	if(parent) {
		const glm::quat parentGlobal = parent->GetGlobalRotation();
		// Local = inverse(parent) * desiredGlobal
		this->rotation = glm::inverse(parentGlobal) * rot;
	} else {
		this->rotation = glm::normalize(rot);
	}
}
void Transform::SetGlobalEulerAngles(glm::vec3 angles) {
	SetGlobalRotation(glm::quat(angles));
}
glm::vec3 Transform::Up() {
	return GetGlobalRotation() * glm::vec3(0.0f, 1.0f, 0.0f);
}
glm::vec3 Transform::Forward() {
	return GetGlobalRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
}
glm::vec3 Transform::Right() {
	return GetGlobalRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
}
glm::vec3 Transform::Down() {
	return GetGlobalRotation() * glm::vec3(0.0f, -1.0f, 0.0f);
}
glm::vec3 Transform::Back() {
	return GetGlobalRotation() * glm::vec3(0.0f, 0.0f, -1.0f);
}
glm::vec3 Transform::Left() {
	return GetGlobalRotation() * glm::vec3(-1.0f, 0.0f, 0.0f);
}