#include <PaperStocking/graphics/camera.hpp>

using namespace PaperStocking;

const glm::mat4 Camera::GetProjMatrix() {
    return glm::ortho(-size, size, -size, size);
}

const glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(transform.GetGlobalPosition(), transform.Forward(), transform.Up());
}
