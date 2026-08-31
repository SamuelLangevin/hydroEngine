#ifndef WAVES_HPP
#define WAVES_HPP
#include <vec2.hpp>

#include "Shader.hpp"

/** \class Wave
 * Base wave attributes and functions.
 */
class Wave {
protected:
    float waveLength{}; /**< The length between two crests or two "highs". */
    float amplitude{}; /**< The height of the wave. */
    float speed{}; /**< how fast the wave fronts travel. */
public:

    /**
     * Base wave constructor
     * @param waveLength
     * @param amplitude
     * @param speed
     */
    explicit Wave(float waveLength, float amplitude, float speed);
    virtual ~Wave() = default;

    [[nodiscard]] float getWaveLength() const;
    [[nodiscard]] float getAmplitude() const;
    [[nodiscard]] float getSpeed() const;

    /**
     * Sets the wavelength. If wavelength < 0.0, will be set to 0.2.
     * @param waveLength
     */
    void setWaveLength(float waveLength);

    /**
     * Sets the amplitude. If amplitude < 0.0, will be set to 0.0.
     * @param amplitude
     */
    void setAmplitude(float amplitude);

    /**
     * Sets the Speed. If speed < 0.0, will be set to 0.0.
     * @param speed
     */
    void setSpeed(float speed);

    /**
     * Every wave needs to be sent to the shader for computation.
     * Sends the wave's attributes to the shader.
     * @param shader to send data for computation
     * @param name of the wave struct instance
     */
    virtual void setUniforms(Shader * shader, const std::string &name) const = 0;
};


/** \class DirectionalWave
 * A one direction wave front across the whole water surface.
 */
class DirectionalWave : public Wave {
protected:
    glm::vec2 direction; /**< The direction the wave front goes in. */

public:

    /**
     * Non default constructor.
     * @param direction
     * @param waveLength defaults to 1.0f
     * @param amplitude defaults to 0.5f
     * @param speed defaults to 3.0f
     */
    explicit DirectionalWave(glm::vec2 direction, float waveLength = 1.0f, float amplitude = 0.5f, float speed = 3.0f);

    [[nodiscard]] glm::vec2 getDirection() const;

    void setDirection(glm::vec2 direction);

    /**
     * Sends the wave's attributes to the shader.
     *
     * Assumes the DirectionalWave struct is from waveFunctions.glsl
     * @param shader to send the data for computing
     * @param name of the wave struct instance
     */
    void setUniforms(Shader * shader, const std::string &name) const override;
};

/** \class PointWave
 * An omnidirectional ripple like wave.
 */
class PointWave : public Wave {
protected:
    float lifetime; /**< The lifetime of a pointWave's effect. Calculated from its amplitude. */
    float dropTime; /**< The absolute time its effect starts. */
    glm::vec2 origin; /**< or epicenter*/

public:


    /**
     * Non default constructor.
     * @param origin
     * @param dropTime
     * @param waveLength
     * @param amplitude
     * @param speed
     */
    explicit PointWave(glm::vec2 origin, float dropTime, float waveLength = 5.0f, float amplitude = 4.0f, float speed = 10.0f);

    [[nodiscard]] glm::vec2 getOrigin() const;
    [[nodiscard]] float getLifetime() const;
    [[nodiscard]] float getDropTime() const;


    /**
     * Sends the wave's attributes to the shader.
     *
     * Assumes the PointWave struct is from waveFunctions.glsl
     * @param shader to send the data for computing
     * @param name of the wave struct instance
     */
    void setUniforms(Shader * shader, const std::string &name) const override;
};


#endif
