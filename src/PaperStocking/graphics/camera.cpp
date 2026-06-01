#include <PaperStocking/graphics/camera.hpp>

using namespace PaperStocking;

const glm::mat4 Camera::GetProjMatrix() {
    return glm::ortho(-size, size, -size, size);
}

const glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 position = transform.GetGlobalPosition();
    glm::vec3 forward = transform.Forward();
    glm::vec3 up = transform.Up();

    glm::vec3 center = position + forward;
    return glm::lookAt(position, center, up);
}
