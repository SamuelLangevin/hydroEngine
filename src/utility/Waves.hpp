#ifndef WAVES_HPP
#define WAVES_HPP
#include <vec2.hpp>


class Shader;

class Wave {
public:
    float waveLength;
    float magnitude;
    float speed;

    explicit Wave(const float waveLength, const float magnitude, const float speed)
        : waveLength(waveLength), magnitude(magnitude), speed(speed){}
    virtual ~Wave() = default;
    virtual void setUniforms(Shader * shader, int index) const = 0;
};

class DirectionalWave : public Wave {
public:
    glm::vec2 direction;

    explicit DirectionalWave(glm::vec2 direction, float waveLength = 0.1f, float magnitude = 0.5f, float speed = 3.0f);
    void setUniforms(Shader * shader, int index) const override;
};

class PointWave : public Wave {
public:
    static constexpr float lifeTime = 5.0f;
    glm::vec2 origin;
    float dropTime;

    explicit PointWave(glm::vec2 position, float dropTime, float waveLength = 0.05f, float magnitude = 0.5f, float speed = 10.0f);
    void setUniforms(Shader * shader, int index) const override;
};


#endif
