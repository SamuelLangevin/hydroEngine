#ifndef WATER_SURFACE_HPP
#define WATER_SURFACE_HPP
#include <memory>
#include <vector>

#include "Entity.hpp"
#include "Waves.hpp"
#include "draw/Surface.hpp"

/**
 * Encapsulates the surface object and the waves of the water.
 */
class WaterSurface : Entity {
private:
    std::vector<std::shared_ptr<DirectionalWave>> directionalWaves; /**< The directional waves present in this water body. */
    std::vector<std::shared_ptr<PointWave>> pointWaves; /**< The point waves present in this water body. */
    std::unique_ptr<Surface> surface; /**< The water mesh. */
    float depth = 0; /**< depth of the water body : [0,1] */

public:
    WaterSurface();

    /** Erases the directional wave at the index's position.*/
    void eraseDirectionalWaveAt(int index);

    /** Deletes the waves past their lifetime. */
    void deleteDeadWaves(float absoluteTime);

    /** Adds the point wave to the water surface */
    void addPointWave(const PointWave &pointWave);

    /** Adds the point wave to the water */
    void addDirectionalWave(const DirectionalWave & directionalWave);

    /** Sets the body of water clamped to [0,1]. */
    void setWaterDepth(float depth);
    [[nodiscard]] float getWaterDepth() const;

    /** @returns the directional wave at the index's position.*/
    std::shared_ptr<DirectionalWave> getDirectionalWaveAt(int index);

    /** Deletes all directional waves. */
    void clearDirectionalWaves();

    /** Deletes all point waves. */
    void clearPointWaves();

    /** @returns the world position of the body of water.*/
    [[nodiscard]] glm::vec3 getPosition() const;

    /** @returns the number of directional waves in the body of water. */
    [[nodiscard]] int getNbOfDirWaves() const;

    /**
     * @param absoluteTime
     * @param initialPosition of the objects (without any water displacement)
     * @returns the resulting position displacement of the waves.
     */
    [[nodiscard]] glm::vec3 computeResultingPosition(float absoluteTime, glm::vec3 initialPosition) const;

    /**
     * @param absoluteTime
     * @param initialPosition of the objects (without any water displacement)
     * @returns the resulting orientation change from the waves.
     */
    [[nodiscard]] glm::vec3 computeResultingNormal(float absoluteTime, glm::vec3 initialPosition) const;

    /** Draws the water surface. */
    void draw(const Shader & shader) const override;
};


#endif
