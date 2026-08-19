#include "Waves.hpp"

#include "Shader.hpp"

DirectionalWave::DirectionalWave(const glm::vec2 direction) : direction(normalize(direction)){}

void DirectionalWave::setUniforms(Shader * shader) {
    shader->setFloat("dWave.waveLength", waveLength);
    shader->setFloat("dWave.magnitude", magnitude);
    shader->setFloat("dWave.speed", speed);
    shader->setVec2("dWave.direction", direction);
}



PointWave::PointWave(const glm::vec2 position) : position(position){}

void PointWave::setUniforms(Shader * shader) {

}
