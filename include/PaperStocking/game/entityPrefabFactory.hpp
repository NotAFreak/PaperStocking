#pragma once
#include <map>
#include <string>
#include <functional>
#include <glm/glm.hpp>

#include <entt/entt.hpp>
#include <PaperStocking/math.hpp>

namespace PaperStocking {
    struct EntityInfo {
        public:
            std::map<std::string, std::string> properties;
            Vector2 origin = {0.0f, 0.0f};
            float rotation = 0.0f;
    };
    using EntityCreatorFunc = std::function<void(entt::registry& registry, EntityInfo info)>;
    struct EntityPrefabFactory {
        public:
            static bool HasEntityPrefab(std::string entityName);
            static void RegisterEntityPrefab(std::string entityName, EntityCreatorFunc creatorFunc);

            static void CreateEntity(std::string entityName, EntityInfo& info, entt::registry& registry);
        private:
            inline static std::map<std::string, EntityCreatorFunc> entityRegistry;
    };
}
