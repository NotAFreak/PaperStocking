#include <PaperStocking/graphics/uniform.hpp>
#include <PaperStocking/graphics/texture.hpp>

using namespace PaperStocking;

Uniform::Uniform(std::string name, glm::mat4 value) {
    this->name = name;
    type = Type_Mat4;
    mat4Value = value;
}
Uniform::Uniform(std::string name, glm::vec4 value) {
    this->name = name;
    type = Type_Mat4;
    vec4Value = value;
}
Uniform::Uniform(std::string name, glm::vec3 value) {
    this->name = name;
    type = Type_Mat4;
    vec3Value = value;
}
Uniform::Uniform(std::string name, glm::vec2 value) {
    this->name = name;
    type = Type_Mat4;
    vec2Value = value;
}
Uniform::Uniform(std::string name, float value) {
    this->name = name;
    type = Type_Mat4;
    floatValue = value;
}
Uniform::Uniform(std::string name, int value) {
    this->name = name;
    type = Type_Mat4;
    intValue = value;
}
void PaperStocking::to_json(nlohmann::json& j, const Uniform& data) {
    switch (data.type) {
        case Type_Mat4:
            j = nlohmann::json {
            {"type", data.type},
            {"name", data.name}
            };
            break;
        case Type_Vec4:
            break;
        case Type_Vec3:
            break;
        case Type_Vec2:
            break;
        case Type_Float:
            break;
        case Type_Int:
            break;
        case Type_Texture:
            j = nlohmann::json {
            {"type", data.type},
            {"name", data.name},
            // {"value", data.textureValue->m_Location},
            };
            break;
    }
}
void PaperStocking::from_json(const nlohmann::json& j, Uniform& data) {
    j.at("type").get_to(data.type);
    j.at("name").get_to(data.name);
    switch (data.type) {
        case Type_Mat4:
            break;
        case Type_Vec4:
            break;
        case Type_Vec3:
            break;
        case Type_Vec2:
            break;
        case Type_Float:
            break;
        case Type_Int:
            break;
        case Type_Texture: {
            std::string textureLocation;
            j.at("value").get_to(textureLocation);
            data.textureValue = std::make_shared<Texture>(textureLocation);
            Renderer::QueueTextureUpload(std::shared_ptr<Texture>(data.textureValue));
            }
            break;
    }
}