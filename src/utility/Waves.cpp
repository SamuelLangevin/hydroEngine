#include "Waves.hpp"

#include "Shader.hpp"

DirectionalWave::DirectionalWave(const glm::vec2 direction, const float waveLength, const float amplitude, const float speed)
    : Wave(waveLength, amplitude, speed),direction(normalize(direction)){}

void DirectionalWave::setUniforms(Shader * shader, const std::string &name) const {
    shader->setFloat(name + ".waveLength", waveLength);
    shader->setFloat(name +".amplitude", amplitude);
    shader->setFloat(name + ".speed", speed);
    shader->setVec2(name + ".direction", direction);
}

PointWave::PointWave(const glm::vec2 origin, const float dropTime, const float waveLength, const float amplitude, const float speed)
    : Wave(waveLength, amplitude, speed), origin(origin), dropTime(dropTime), lifetime(3.0f * amplitude){}

void PointWave::setUniforms(Shader * shader, const std::string &name) const{
    shader->setFloat(name+ ".waveLength", waveLength);
    shader->setFloat(name+ ".amplitude", amplitude);
    shader->setFloat(name+ ".speed", speed);
    shader->setVec2(name+ ".origin", origin);
    shader->setFloat(name+ ".dropTime", dropTime);
}