#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <mutex>
#include <PaperStocking/graphics/uniform.hpp>
#include <PaperStocking/graphics/shader/shaderProgram.hpp>
#include <nlohmann/json.hpp>
namespace PaperStocking {
    struct Material {
        public:
            std::string shaderName;
            std::shared_ptr<ShaderProgram> shader;
            
            std::vector<Uniform> uniforms;

            Material(Material&&) = default;
            Material(std::string shaderName);
            Material(std::shared_ptr<ShaderProgram> shader);
    };
    // Used for saving and loading information about the material
    struct MaterialData {
        public:
            std::string shader;
            std::vector<Uniform> uniforms;

            MaterialData() = default;
            MaterialData(const MaterialData&) = delete;
            MaterialData(MaterialData&&) = default;
    };
    inline void to_json(nlohmann::json& j, const MaterialData& data) {
        j = nlohmann::json {
        {"shader", data.shader},
        {"uniforms", data.uniforms}
        };
    }
    inline void from_json(const nlohmann::json& j, MaterialData& data) {
        j.at("shader").get_to(data.shader);
        j.at("uniforms").get_to(data.uniforms);
    }
}
