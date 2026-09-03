#include "SceneManager.hpp"

#include <iostream>

#include "../utility/Utility.hpp"
#include "../utility/ResourceManager.hpp"
#include "../draw/Cube.hpp"
#include "../repositories/SceneRepository.hpp"

void SceneManager::init() {
    using SR = SceneRepository;

    Surface * waterSurface = new Surface(glm::ivec2(1000));
    waterSurface->scale = glm::vec3(1.0f);
    waterSurface->position = glm::vec3(0.0f, -20.0f, 0.0f);
    waterSurface->material.metallic = 1.0;
    waterSurface->material.roughness = 0.0f;
    waterSurface->material.ao = 1.0f;
    waterSurface->material.texture_diffuse0 = ResourceManager::getTexture("deepBlue");
    SR::water = waterSurface;

    SR::directionalWaves.emplace_back(glm::vec2(0.721f, 0.693f), 1.0f, 0.2f, 3.0);
    SR::directionalWaves.emplace_back(glm::vec2(0.275f, 0.962f), 2.0f, 0.6f, 4.0);
    SR::directionalWaves.emplace_back(glm::vec2(0.0f, 1.0f), 0.6f, 0.2f, 1.0);
    SR::directionalWaves.emplace_back(glm::vec2(-0.275f, 0.962f), 12.0f, 2.0f, 3.0);
    SR::directionalWaves.emplace_back(glm::vec2(0.5, 0.5), 8.0f, 1.5f, 3.0);

    Cube * cube = new Cube();
    cube->material.texture_diffuse0 = ResourceManager::getTexture("red");
    cube->scale = glm::vec3(3.0f);
    SR::entities.push_back(cube);
}

void SceneManager::free() {
    SceneRepository::free();
}

void SceneManager::update(float time, glm::vec2 windowSize, const Camera & camera) {
    deleteDeadWaves(time);

    worldCursorPos = Utility::getClickPositionOnPlane(0.5f * glm::vec2(windowSize), camera,
                SceneRepository::water->position, glm::vec3(0.0f, 1.0f, 0.0f), windowSize);

    displaceObjects(time);
}


void SceneManager::produceWave(const Camera & camera, float currentTime, glm::ivec2 windowSize, const PointWave & pointWaveParameters) {
    const glm::vec2 screenCenter = 0.5f * glm::vec2(windowSize);
    glm::vec3 nearClipClick = camera.screenClickToNearClip(screenCenter, windowSize);
    if (glm::dot(camera.getPosition() - nearClipClick, camera.getPosition() - worldCursorPos) > 0.0f) {
        PointWave wave(glm::vec2(worldCursorPos.x, worldCursorPos.z), currentTime,
            pointWaveParameters.getWaveLength(), pointWaveParameters.getAmplitude(), pointWaveParameters.getSpeed());
        SceneRepository::pointWaves.push_back(wave);
    }
}

void SceneManager::deleteDeadWaves(float time) {
    using SR = SceneRepository;

    for (uint i = 0; i < SR::pointWaves.size(); ++i){
        const PointWave & wave = SR::pointWaves[i];
        if (time - wave.getDropTime() > wave.getLifetime())
            SR::pointWaves.erase(SR::pointWaves.begin() + i);
        else break;
    }
}

void SceneManager::displaceObjects(float time) {
    using SR = SceneRepository;

    for (auto & entity : SR::entities) {
        glm::vec3 newPosition = SR::water->position;
        glm::vec3 binormal = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 1.0f);

        for (auto & pWave : SR::pointWaves) {
            newPosition += pWave.computeDisplacement(time, entity->position);
            auto binormalAndTangent = pWave.computeBinormalAndTangent(time, entity->position);
            binormal += binormalAndTangent.first;
            tangent += binormalAndTangent.second;
        }
        for (auto & dWave : SR::directionalWaves) {
            newPosition += dWave.computeDisplacement(time, entity->position);
            auto binormalAndTangent = dWave.computeBinormalAndTangent(time, entity->position);
            binormal += binormalAndTangent.first;
            tangent += binormalAndTangent.second;
        }
        glm::vec3 normal = glm::normalize(glm::cross(binormal, tangent));

        float dot = glm::dot(glm::normalize(normal), glm::vec3(0.0f, -1.0f, 0.0f));
        entity->orientation = glm::mix(entity->orientation, getLookAtQuat(normal), 1.0f);
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