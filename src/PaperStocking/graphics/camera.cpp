#include <PaperStocking/graphics/camera.hpp>
#include <PaperStocking/graphics/renderer.hpp>

using namespace PaperStocking;

const glm::mat4 Camera::GetProjMatrix() {
    glm::vec2 resolution = Renderer::GetRenderResolution();
    float aspectRatio = resolution.x / resolution.y;
    return glm::ortho(-size * aspectRatio, size * aspectRatio, -size, size, near, far);
}

const glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 position = transform.GetGlobalPosition();
    glm::vec3 forward = transform.Forward();
    glm::vec3 up = transform.Up(); //// mmmmmm

    glm::vec3 center = position + forward;
    return glm::lookAt(position, center, up);
}
