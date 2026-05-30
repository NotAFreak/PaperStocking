#pragma once
#include <PaperStocking/game/transform.hpp>
namespace PaperStocking {
    struct Camera {
        Transform transform;
        float size = 10.0f;
        float near = 0.01f;
        float far = 10.0f;

        const glm::mat4 GetProjMatrix();
        const glm::mat4 GetViewMatrix();
    };
}