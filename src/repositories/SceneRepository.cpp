#include "SceneRepository.hpp"

Surface * SceneRepository::water = nullptr; /**< The water surface's mesh.*/
std::vector<DirectionalWave> SceneRepository::directionalWaves;
std::vector<PointWave> SceneRepository::pointWaves;
std::vector<BasicShape*> SceneRepository::entities;

void SceneRepository::eraseDirWave(int index) {
    directionalWaves.erase(directionalWaves.begin() + index);
}

void SceneRepository::free() {
    delete water;
    for (auto entity: entities) {
        delete entity;
    }
}
