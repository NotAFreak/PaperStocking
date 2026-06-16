#pragma once

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

namespace PaperStocking {
    struct Transform {
        Transform* parent = nullptr;

        // Local position
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        // Local rotation
        glm::quat rotation = glm::identity<glm::quat>();
        // Local scale
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};

        Transform() = default;
        Transform(glm::vec3 pos);
        Transform(glm::vec3 pos, glm::quat rotation);
        Transform(glm::vec3 pos, glm::vec3 scale);
        Transform(glm::vec3 pos, glm::quat rotation, glm::vec3 scale);

        inline bool operator==(const Transform& other) {
            return 
                this == &other &&
                other.position == this->position &&
                other.parent == this->parent && 
                other.rotation == this->rotation &&
                other.scale == this->scale;
        }
        
        glm::mat4 GetLocalMatrix();
        glm::mat4 GetMatrix();

        glm::vec3 GetGlobalPosition();
        glm::quat GetGlobalRotation();
        glm::vec3 GetGlobalEulerAngles();
        
        void SetGlobalPosition(glm::vec3 pos);
        void SetGlobalRotation(glm::quat rot);
        void SetGlobalEulerAngles(glm::vec3 angles);

        glm::vec3 Up();
        glm::vec3 Forward();
        glm::vec3 Right();
        glm::vec3 Down();
        glm::vec3 Back();
        glm::vec3 Left();
    };
}
