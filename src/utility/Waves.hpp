#ifndef WAVES_HPP
#define WAVES_HPP
#include <vec2.hpp>


class Shader;

class Wave {
public:
    float waveLength;
    float magnitude;
    float speed;

    explicit Wave(const float waveLength = 0.1f, const float magnitude = 0.5f, const float speed = 3.0f)
        : waveLength(waveLength), magnitude(magnitude), speed(speed){}
    virtual ~Wave() = default;
    virtual void setUniforms(Shader * shader) = 0;
};

class DirectionalWave : public Wave {
public:
    glm::vec2 direction;

    explicit DirectionalWave(glm::vec2 direction = glm::vec2(1.0f, 0.0f));
    void setUniforms(Shader * shader) override;
};

class PointWave : public Wave {
public:
    glm::vec2 position;

    explicit PointWave(glm::vec2 position = glm::vec2(0.5f, 0.5f));
    void setUniforms(Shader * shader) override;
};


#endif
