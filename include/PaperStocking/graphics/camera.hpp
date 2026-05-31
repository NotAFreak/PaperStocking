#pragma once
#include <PaperStocking/game/transform.hpp>
namespace PaperStocking {
    struct Camera {
        Transform transform = Transform({0.0f, 0.0f, -10.0f});
        float size = 10.0f;
        float near = 0.01f;
        float far = 50.0f;

        const glm::mat4 GetProjMatrix();
        const glm::mat4 GetViewMatrix();
        const glm::mat4 GetFullMatrix() {
            return GetProjMatrix() * GetViewMatrix();
        }
    };
}