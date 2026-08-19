#include "Lights.hpp"
#include "../draw/Sphere.hpp"
#include "../draw/Cone.hpp"


AmbientLight::AmbientLight(glm::vec3 color): Light(color, AMBIENT) {}

void AmbientLight::draw(Shader * shader) {}

void AmbientLight::setUniforms(Shader * shader, int lightIndex) {}



DirectionalLight::DirectionalLight(): Light(glm::vec3(1.0f), DIRECTIONAL), direction(glm::vec3(1.0f, 0.0f, 0.0f)) {}

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction): Light(color, DIRECTIONAL), direction(direction) {}

void DirectionalLight::draw(Shader * shader) {}

void DirectionalLight::setUniforms(Shader * shader, int lightIndex) {
    shader->use();
    shader->setVec3("dirLights[" + std::to_string(lightIndex) + "].direction",direction);
    shader->setVec3("dirLights[" + std::to_string(lightIndex) + "].color",color);
}



PointLight::PointLight() : Light(glm::vec3(1.0f), POINT) {}

PointLight::PointLight(glm::vec3 color, glm::vec3 position): Light(color, POINT), position(position) {}

void PointLight::draw(Shader * shader) {
    shader->use();
    Sphere lightModel;
    lightModel.position = position;
    lightModel.scale = glm::vec3(0.5f);
    shader->setVec3("color", color);
    lightModel.draw(shader);
}

void PointLight::setUniforms(Shader * shader, int lightIndex) {
    shader->use();
    shader->setVec3("pointLights[" + std::to_string(lightIndex) + "].position",position);
    shader->setVec3("pointLights[" + std::to_string(lightIndex) + "].color", color);
    shader->setFloat("pointLights[" + std::to_string(lightIndex) + "].strength",strength);
}



SpotLight::SpotLight() : Light(glm::vec3(1.0f), SPOT), position(glm::vec3(0.0f)), direction(glm::vec3(1.0f, 0.0f, 0.0f)) {}

SpotLight::SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 direction)
    : Light(color, SPOT), position(position), direction(direction) {}

void SpotLight::draw(Shader * shader) {
    shader->use();
    Cone lightModel;
    lightModel.position = position;
    lightModel.scale = glm::vec3(0.5f);
    lightModel.orientation = getLookAtQuat();
    shader->setVec3("color", color);
    lightModel.draw(shader);
}

void SpotLight::setUniforms(Shader * shader, int lightIndex) {
    shader->use();
    shader->setVec3("spotLights[" + std::to_string(lightIndex) + "].position", position);
    shader->setVec3("spotLights[" + std::to_string(lightIndex) + "].direction", direction);
    shader->setVec3("spotLights[" + std::to_string(lightIndex) + "].color", color);
    shader->setFloat("spotLights[" + std::to_string(lightIndex) + "].strength", strength);
    shader->setFloat("spotLights[" + std::to_string(lightIndex) + "].angularAttenuation", angularAttenuation);
}

glm::quat SpotLight::getLookAtQuat(){
    const glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
    const float angle = acos(glm::dot(glm::normalize(direction), down));
    return glm::quat(-angle, normalize(glm::cross(direction, down)));
}