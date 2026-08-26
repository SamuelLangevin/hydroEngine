#include "Waves.hpp"

#include <ext/quaternion_geometric.hpp>

#include "Shader.hpp"

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



PointWave::PointWave(const glm::vec2 origin, const float dropTime, const float waveLength, const float amplitude, const float speed)
    : Wave(waveLength, amplitude, speed), lifetime(150.0f/(0.1f+ std::max(speed, 0.0f))), origin(origin), dropTime(dropTime) {}

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
