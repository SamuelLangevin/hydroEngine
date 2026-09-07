#include "SceneRepository.hpp"

std::shared_ptr<WaterSurface> SceneRepository::water;
std::vector<std::shared_ptr<Entity>> SceneRepository::entities;

void SceneRepository::initWaterSurface() {
    if (!water) water = std::make_shared<WaterSurface>(WaterSurface());
}

std::shared_ptr<WaterSurface> SceneRepository::getWaterSurface() {
    return water;
}

void SceneRepository::addEntity(const std::shared_ptr<Entity>& entity) {
    entities.push_back(entity);
}

std::shared_ptr<Entity> SceneRepository::getEntity(int index) {
    return entities.at(index);
}

void SceneRepository::drawEntities(const Shader &shader) {
    for (const auto& entity: entities) entity->draw(shader);
}

void SceneRepository::free() {
    entities.clear();
    water.reset();
}

int SceneRepository::getNbOfEntities() {
    return static_cast<int>(entities.size());
}
