#pragma once
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <PaperStocking/graphics/texture.hpp>
#include <PaperStocking/graphics/renderer.hpp>

namespace PaperStocking {
    enum UniformType : uint8_t {
        Type_Mat4,
        Type_Vec4,
        Type_Vec3,
        Type_Vec2,
        Type_Float,
        Type_Int,
        Type_Texture
    };
    struct Uniform {
        public:
            UniformType type = Type_Int;
            std::string name = "???";
            union {
                glm::mat4 mat4Value;
                glm::vec4 vec4Value;
                glm::vec3 vec3Value;
                glm::vec2 vec2Value;
                float floatValue;
                int intValue;
            };
            std::shared_ptr<Texture> textureValue;

            // ONLY USED FOR SERIALIZATION/DESERIALIZATION. DO NOT USE.
            Uniform() = default;
            ~Uniform() = default;

            Uniform(std::string name, glm::mat4 value);
            Uniform(std::string name, glm::vec4 value);
            Uniform(std::string name, glm::vec3 value);
            Uniform(std::string name, glm::vec2 value);
            Uniform(std::string name, float value);
            Uniform(std::string name, int value);
            // Uniform(std::string name, std::shared_ptr<Texture> value, uint slot);
    };
    void to_json(nlohmann::json& j, const Uniform& data);
    void from_json(const nlohmann::json& j, Uniform& data);
}
