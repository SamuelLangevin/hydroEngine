#include "SceneManager.hpp"

#include <iostream>

#include "../utility/Utility.hpp"
#include "../utility/ResourceManager.hpp"
#include "../draw/Sphere.hpp"

void SceneManager::init() {

    scene.water = new Surface(glm::ivec2(1000));
    scene.water->scale = glm::vec3(1.0f);
    scene.water->position = glm::vec3(0.0f, -20.0f, 0.0f);
    scene.water->material.metallic = 1.0;
    scene.water->material.roughness = 0.0f;
    scene.water->material.ao = 1.0f;
    scene.water->material.texture_diffuse0 = ResourceManager::getTexture("deepBlue");

    scene.directionalWaves.emplace_back(glm::vec2(0.721f, 0.693f), 1.0f, 0.2f, 3.0);
    scene.directionalWaves.emplace_back(glm::vec2(0.275f, 0.962f), 2.0f, 0.6f, 4.0);
    scene.directionalWaves.emplace_back(glm::vec2(0.0f, 1.0f), 0.6f, 0.2f, 1.0);
    scene.directionalWaves.emplace_back(glm::vec2(-0.275f, 0.962f), 12.0f, 2.0f, 3.0);
    scene.directionalWaves.emplace_back(glm::vec2(0.5, 0.5), 8.0f, 1.5f, 3.0);

    Sphere * sphere = new Sphere();
    sphere->material.texture_diffuse0 = ResourceManager::getTexture("red");
    sphere->scale = glm::vec3(3.0f);
    scene.entities.push_back(sphere);
}

void SceneManager::free() {
    delete scene.water;
    for (auto entity: scene.entities) {
        delete entity;
    }
}

void SceneManager::update(float time, glm::vec2 windowSize, const Camera & camera) {
    deleteDeadWaves(time);

    worldCursorPos = Utility::getClickPositionOnPlane(0.5f * glm::vec2(windowSize), camera,
                scene.water->position, glm::vec3(0.0f, 1.0f, 0.0f), windowSize);

    displaceObjects(time);
}


void SceneManager::produceWave(const Camera & camera, float currentTime, glm::ivec2 windowSize, const PointWave & pointWaveParameters) {
    const glm::vec2 screenCenter = 0.5f * glm::vec2(windowSize);
    glm::vec3 nearClipClick = camera.screenClickToNearClip(screenCenter, windowSize);
    if (glm::dot(camera.getPosition() - nearClipClick, camera.getPosition() - worldCursorPos) > 0.0f) {
        PointWave wave(glm::vec2(worldCursorPos.x, worldCursorPos.z), currentTime,
            pointWaveParameters.getWaveLength(), pointWaveParameters.getAmplitude(), pointWaveParameters.getSpeed());
        scene.pointWaves.push_back(wave);
    }
}

void SceneManager::deleteDeadWaves(float time) {

    for (uint i = 0; i < scene.pointWaves.size(); ++i){
        const PointWave & wave = scene.pointWaves[i];
        if (time - wave.getDropTime() > wave.getLifetime())
            scene.pointWaves.erase(scene.pointWaves.begin() + i);
        else break;
    }
}

void SceneManager::displaceObjects(float time) {
    for (auto & entity : scene.entities) {
        glm::vec3 newPosition = scene.water->position;
        glm::vec3 binormal = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 1.0f);

        for (auto & pWave : scene.pointWaves) {
            newPosition += pWave.computeDisplacement(time, entity->position);
            auto binormalAndTangent = pWave.computeBinormalAndTangent(time, entity->position);
            binormal += binormalAndTangent.first;
            tangent += binormalAndTangent.second;
        }
        for (auto & dWave : scene.directionalWaves) {
            newPosition += dWave.computeDisplacement(time, entity->position);
            auto binormalAndTangent = dWave.computeBinormalAndTangent(time, entity->position);
            binormal += binormalAndTangent.first;
            tangent += binormalAndTangent.second;
        }

        entity->waterDis = newPosition;
    }
}

const SceneManager::Scene & SceneManager::getScene() const {return scene;}

glm::vec3 SceneManager::getCursorWorldPos() const {
    return worldCursorPos;
}

std::vector<DirectionalWave> * SceneManager::getDirWavesPointer() {
    return &scene.directionalWaves;
}

std::vector<PointWave> * SceneManager::getPntWavesPointer() {
    return &scene.pointWaves;
}
