#include "Waves.hpp"

#include "Shader.hpp"

DirectionalWave::DirectionalWave(const glm::vec2 direction, const float waveLength, const float magnitude, const float speed)
    : Wave(waveLength, magnitude, speed),direction(normalize(direction)){}

void DirectionalWave::setUniforms(Shader * shader, const std::string &name) const {
    shader->setFloat(name + ".waveLength", waveLength);
    shader->setFloat(name +".magnitude", magnitude);
    shader->setFloat(name + ".speed", speed);
    shader->setVec2(name + ".direction", direction);
}

PointWave::PointWave(const glm::vec2 origin, const float dropTime, const float waveLength, const float magnitude, const float speed)
    : Wave(waveLength, magnitude, speed), origin(origin), dropTime(dropTime){}

void PointWave::setUniforms(Shader * shader, const std::string &name) const{
    shader->setFloat(name+ ".waveLength", waveLength);
    shader->setFloat(name+ ".magnitude", magnitude);
    shader->setFloat(name+ ".speed", speed);
    shader->setVec2(name+ ".origin", origin);
    shader->setFloat(name+ ".dropTime", dropTime);
}
//"pointWaves[" + std::to_string(index) + "]"