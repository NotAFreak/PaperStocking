#include <PaperStocking/game/entityPrefabFactory.hpp>

using namespace PaperStocking;

bool EntityPrefabFactory::HasEntityPrefab(std::string entityName) {
    return entityRegistry.contains(entityName);
}

void EntityPrefabFactory::RegisterEntityPrefab(std::string entityName, EntityCreatorFunc creatorFunc) {
    assert(!HasEntityPrefab(entityName));
    entityRegistry.emplace(entityName, creatorFunc);
}

void EntityPrefabFactory::CreateEntity(std::string entityName, EntityInfo& info, entt::registry &registry) {
    assert(HasEntityPrefab(entityName));
    entityRegistry[entityName](registry, info);
}

