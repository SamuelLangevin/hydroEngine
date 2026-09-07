#include "SceneRepository.hpp"

WaterSurface* SceneRepository::water = nullptr;
std::vector<Entity*> SceneRepository::entities;

void SceneRepository::initWaterSurface() {
    if (!water) water = new WaterSurface();
}

WaterSurface* SceneRepository::getWaterSurface() {
    return water;
}

void SceneRepository::addEntity(Entity * entity) {
    entities.push_back(entity);
}

Entity* SceneRepository::getEntity(int index) {
    return entities.at(index);
}

void SceneRepository::drawEntities(const Shader &shader) {
    for (auto entity: entities) entity->draw(shader);
}

void SceneRepository::free() {
    for (auto entity: entities) {
        delete entity;
    }
}

int SceneRepository::getNbOfEntities() {
    return static_cast<int>(entities.size());
}
