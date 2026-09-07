#include "SceneManager.hpp"

#include <iostream>
#include <memory>

#include "../resources/Utility.hpp"
#include "../repositories/ResourceRepository.hpp"
#include "../scene/draw/Cube.hpp"
#include "../repositories/SceneRepository.hpp"

void SceneManager::init() {
    using SR = SceneRepository;

    SR::initWaterSurface();

    Cube cube;
    cube.material.texture_diffuse0 = ResourceRepository::getTexture("red");
    cube.scale = glm::vec3(3.0f);
    SR::addEntity(std::make_shared<Cube>(cube));
}

void SceneManager::free() {
    SceneRepository::free();
}

void SceneManager::update(float time, glm::vec2 windowSize, const Camera & camera) {
    std::shared_ptr<WaterSurface> water = SceneRepository::getWaterSurface();
    water->deleteDeadWaves(time);

    worldCursorPos = Utility::getClickPositionOnPlane(0.5f * glm::vec2(windowSize), camera,
                water->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f), windowSize);

    displaceObjects(time);
}


void SceneManager::produceWave(const Camera & camera, float currentTime, glm::ivec2 windowSize, const PointWave & pointWaveParameters) {
    const glm::vec2 screenCenter = 0.5f * glm::vec2(windowSize);
    glm::vec3 nearClipClick = camera.screenClickToNearClip(screenCenter, windowSize);
    if (glm::dot(camera.getPosition() - nearClipClick, camera.getPosition() - worldCursorPos) > 0.0f) {
        PointWave wave(glm::vec2(worldCursorPos.x, worldCursorPos.z), currentTime,
            pointWaveParameters.getWaveLength(), pointWaveParameters.getAmplitude(), pointWaveParameters.getSpeed());
        SceneRepository::getWaterSurface()->addPointWave(wave);
    }
}

void SceneManager::displaceObjects(float time) {
    using SR = SceneRepository;
    std::shared_ptr<WaterSurface> water = SR::getWaterSurface();


    for (int i = 0; i < SR::getNbOfEntities(); ++i) {
        std::shared_ptr<Entity> entity = SR::getEntity(i);
        glm::vec3 newPosition = water->computeResultingPosition(time, entity->position);
        glm::vec3 newNormal = water->computeResultingNormal(time, entity->position);

        entity->orientation = glm::mix(entity->orientation, getLookAtQuat(newNormal), 1.0f);
        entity->waterDis = newPosition;
    }
}

glm::quat SceneManager::getLookAtQuat(glm::vec3 direction) {
    glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
    float dot = glm::dot(glm::normalize(direction), down);
    if (-1.0f < dot && dot < 1.0f) {
        return {-std::acos(dot), normalize(glm::cross(direction, down))};
    }
    return {0.0f, glm::vec3(-1.0f, 0.0f, 0.0f)};

}


glm::vec3 SceneManager::getCursorWorldPos() const {
    return worldCursorPos;
}