#include "WaterSurface.hpp"
#include "../repositories/ResourceRepository.hpp"

WaterSurface::WaterSurface(){
    directionalWaves.emplace_back(glm::vec2(0.721f, 0.693f), 1.0f, 0.2f, 3.0);
    directionalWaves.emplace_back(glm::vec2(0.275f, 0.962f), 2.0f, 0.6f, 4.0);
    directionalWaves.emplace_back(glm::vec2(0.0f, 1.0f), 0.6f, 0.2f, 1.0);
    directionalWaves.emplace_back(glm::vec2(-0.275f, 0.962f), 12.0f, 2.0f, 3.0);
    directionalWaves.emplace_back(glm::vec2(0.5, 0.5), 8.0f, 1.5f, 3.0);

    surface = new Surface(glm::ivec2(1000));

    surface->scale = glm::vec3(1.0f);
    surface->position = glm::vec3(0.0f, -0.0f, 0.0f);
    surface->material.metallic = 1.0;
    surface->material.roughness = 0.0f;
    surface->material.ao = 1.0f;
    surface->material.texture_diffuse0 = ResourceRepository::getTexture("deepBlue");
}

void WaterSurface::eraseDirWave(int index) {
    directionalWaves.erase(directionalWaves.begin() + index);
}

void WaterSurface::deleteDeadWaves(float absoluteTime) {
    for (uint i = 0; i < pointWaves.size(); ++i){
        const PointWave & wave = pointWaves[i];
        if (absoluteTime - wave.getDropTime() > wave.getLifetime())
            pointWaves.erase(pointWaves.begin() + i);
        else break;
    }
}

void WaterSurface::addPointWave(const PointWave &pointWave) {
    pointWaves.push_back(pointWave);
}

void WaterSurface::addDirectionalWave(const DirectionalWave &directionalWave) {
    directionalWaves.push_back(directionalWave);
}

DirectionalWave * WaterSurface::getDirectionalWave(int index) {
    return &directionalWaves.at(index);
}

void WaterSurface::clearDirectionalWaves() { directionalWaves.clear();}

void WaterSurface::clearPointWaves() {pointWaves.clear(); }

glm::vec3 WaterSurface::getPosition() const {
    return surface->position;
}

int WaterSurface::getNbOfDirWaves() const {
    return static_cast<int>(directionalWaves.size());
}

glm::vec3 WaterSurface::computeResultingPosition(float absoluteTime, glm::vec3 initialPosition) const {
    glm::vec3 newPosition = initialPosition;

    for (auto & pWave : pointWaves) {
        newPosition += pWave.computeDisplacement(absoluteTime, initialPosition);
    }
    for (auto & dWave : directionalWaves) {
        newPosition += dWave.computeDisplacement(absoluteTime, initialPosition);
    }
    return newPosition;
}

glm::vec3 WaterSurface::computeResultingNormal(float absoluteTime, glm::vec3 initialPosition) const {
    glm::vec3 binormal = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 1.0f);

    for (auto & pWave : pointWaves) {
        auto binormalAndTangent = pWave.computeBinormalAndTangent(absoluteTime, initialPosition);
        binormal += binormalAndTangent.first;
        tangent += binormalAndTangent.second;
    }
    for (auto & dWave : directionalWaves) {
        auto binormalAndTangent = dWave.computeBinormalAndTangent(absoluteTime, initialPosition);
        binormal += binormalAndTangent.first;
        tangent += binormalAndTangent.second;
    }
    return glm::normalize(glm::cross(binormal, tangent));
}

void WaterSurface::draw(const Shader & shader) const {
    shader.setInt("nbOfPointWaves", static_cast<int>(pointWaves.size()));
    for (int i = 0; i < std::min(static_cast<int>(pointWaves.size()), 50); ++i) {
        pointWaves.at(i).setUniforms(shader, "pointWaves[" + std::to_string(i) + "]");
    }
    shader.setInt("nbOfDirectionalWaves", static_cast<int>(directionalWaves.size()));
    for (int i = 0; i < std::min(static_cast<int>(directionalWaves.size()), 50); ++i) {
        directionalWaves.at(i).setUniforms(shader, "directionalWaves[" + std::to_string(i) + "]");
    }
    surface->draw(shader);
}
