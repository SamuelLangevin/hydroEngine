#include "Waves.hpp"

#include <ext/quaternion_geometric.hpp>

#include "Shader.hpp"

constexpr float PI = 3.14159;
constexpr float e = 2.71828;

Wave::Wave(const float waveLength, const float amplitude, const float speed){
    setWaveLength(waveLength);
    setAmplitude(amplitude);
    setSpeed(speed);
}

float Wave::getWaveLength() const { return waveLength; }
float Wave::getAmplitude() const { return amplitude; }
float Wave::getSpeed() const { return speed; }

void Wave::setWaveLength(const float waveLength) {this->waveLength = std::max(waveLength, 0.2f);}
void Wave::setAmplitude(const float amplitude) { this->amplitude = std::max(amplitude, 0.0f); }
void Wave::setSpeed(const float speed) { this->speed = std::max(speed, 0.0f); }



DirectionalWave::DirectionalWave(const glm::vec2 direction, const float waveLength, const float amplitude, const float speed)
    : Wave(waveLength, amplitude, speed), direction(normalize(direction)){}

glm::vec2 DirectionalWave::getDirection() const { return direction; }

void DirectionalWave::setDirection(glm::vec2 direction) { this->direction = normalize(direction); }

void DirectionalWave::setUniforms(Shader * shader, const std::string &name) const {
    shader->setFloat(name + ".waveLength", waveLength);
    shader->setFloat(name +".amplitude", amplitude);
    shader->setFloat(name + ".speed", speed);
    shader->setVec2(name + ".direction", direction);
}

std::pair<glm::vec3, glm::vec3> DirectionalWave::computeBinormalAndTangent(float absoluteTime, glm::vec3 position) const {
    float advance = speed*absoluteTime / std::sqrt(waveLength);
    float phase = glm::dot(direction, glm::vec2(position.x, position.z))/waveLength - advance;

    glm::vec3 binormal;
    binormal.x = -amplitude*direction.x/waveLength * std::cos(phase);
    binormal.y = -amplitude*direction.x/waveLength * std::sin(phase);
    binormal.z = 0.0;

    glm::vec3 tangent;
    tangent.x = 0.0;
    tangent.y = -amplitude*direction.y/waveLength * std::sin(phase);
    tangent.z = -amplitude*direction.y/waveLength * std::cos(phase);
    return {binormal, tangent};
}

glm::vec3 DirectionalWave::computeDisplacement(float absoluteTime, glm::vec3 position) const {
    glm::vec3 newPos;
    float advance = speed*absoluteTime / std::sqrt(waveLength);
    float phase = glm::dot(direction, glm::vec2(position.x, position.z))/waveLength - advance;
    newPos.x = -amplitude * direction.x * std::sin(phase);
    newPos.y = amplitude * std::cos(phase);
    newPos.z = -amplitude * direction.y * std::sin(phase);
    return newPos;
}



PointWave::PointWave(const glm::vec2 origin, const float dropTime, const float waveLength, const float amplitude, const float speed)
    : Wave(waveLength, amplitude, speed), lifetime(250.0f*sqrt(std::max(waveLength, 0.2f))/(std::max(speed, 0.1f))), dropTime(dropTime), origin(origin) {}

glm::vec2 PointWave::getOrigin() const { return origin; }
float PointWave::getLifetime() const { return lifetime; }
float PointWave::getDropTime() const { return dropTime; }

void PointWave::setUniforms(Shader * shader, const std::string &name) const{
    shader->setFloat(name+ ".waveLength", waveLength);
    shader->setFloat(name+ ".amplitude", amplitude);
    shader->setFloat(name+ ".speed", speed);
    shader->setVec2(name+ ".origin", origin);
    shader->setFloat(name+ ".dropTime", dropTime);
}

std::pair<glm::vec3, glm::vec3> PointWave::computeBinormalAndTangent(float absoluteTime, glm::vec3 position) const {
    glm::vec2 originToVert = origin - glm::vec2(position.x, position.z);
    float advance = speed * (absoluteTime - dropTime) /std::sqrt(waveLength);
    float reachedPos = ceil(glm::clamp(PI * advance - length(originToVert), 0.0f, 1.0f));
    float attenuation = std::pow(e, -advance/(amplitude * 5.0)) * reachedPos;
    float phase = -length(originToVert)/waveLength + advance;
    float phaseXderivate = (position.x - origin.x) / (sqrt(waveLength) * -length(originToVert));
    float phaseZderivate = (position.z - origin.y) / (sqrt(waveLength) * -length(originToVert));

    glm::vec3 binormal = glm::vec3(0.0, attenuation * phaseXderivate * amplitude * std::sin(phase), 0.0);
    glm::vec3 tangent = glm::vec3(0.0, attenuation * phaseZderivate * amplitude * std::sin(phase), 0.0);
    return {binormal, tangent};
}


glm::vec3 PointWave::computeDisplacement(float absoluteTime, glm::vec3 position) const {
    glm::vec2 originToVert = origin - glm::vec2(position.x, position.z);
    float advance = speed * (absoluteTime - dropTime) /std::sqrt(waveLength);
    float reachedPos = ceil(glm::clamp(PI * advance - glm::length(originToVert), 0.0f, 1.0f));
    float attenuation = std::pow(e, -advance/(amplitude * 5.0)) * reachedPos;
    float phase = -length(originToVert)/waveLength + advance;

    return {0.0, attenuation * amplitude * std::sin(phase), 0.0};
}
