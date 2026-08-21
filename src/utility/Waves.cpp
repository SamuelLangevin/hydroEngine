#include "Waves.hpp"

#include "Shader.hpp"

DirectionalWave::DirectionalWave(const glm::vec2 direction, const float waveLength, const float magnitude, const float speed)
    : Wave(waveLength, magnitude, speed),direction(normalize(direction)){}

void DirectionalWave::setUniforms(Shader * shader, int index) const {
    shader->setFloat("dWave.waveLength", waveLength);
    shader->setFloat("dWave.magnitude", magnitude);
    shader->setFloat("dWave.speed", speed);
    shader->setVec2("dWave.direction", direction);
}

PointWave::PointWave(const glm::vec2 origin, const float dropTime, const float waveLength, const float magnitude, const float speed)
    : Wave(waveLength, magnitude, speed), origin(origin), dropTime(dropTime){}

void PointWave::setUniforms(Shader * shader, int index) const{
    shader->setFloat("pWaves[" + std::to_string(index) + "].waveLength", waveLength);
    shader->setFloat("pWaves[" + std::to_string(index) + "].magnitude", magnitude);
    shader->setFloat("pWaves[" + std::to_string(index) + "].speed", speed);
    shader->setVec2("pWaves[" + std::to_string(index) + "].origin", origin);
    shader->setFloat("pWaves[" + std::to_string(index) + "].dropTime", dropTime);
}
