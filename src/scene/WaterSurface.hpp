#ifndef WATER_SURFACE_HPP
#define WATER_SURFACE_HPP
#include <vector>

#include "Entity.hpp"
#include "Waves.hpp"
#include "draw/Surface.hpp"

/**
 * Encapsulates the surface object and the waves of the water.
 */
class WaterSurface : Entity {
private:
    std::vector<DirectionalWave> directionalWaves; /**< The directional waves present in this water body. */
    std::vector<PointWave> pointWaves; /**< The point waves present in this water body. */
    Surface * surface = nullptr; /**< The water mesh. */

public:
    WaterSurface();

    /**
     * Erases the directional wave at the index's position.
     * @param index
     */
    void eraseDirWave(int index);

    /**
     * Deletes the waves past their lifetime.
     * @param absoluteTime
     */
    void deleteDeadWaves(float absoluteTime);

    /**
     * Adds the point wave.
     * @param pointWave
     */
    void addPointWave(const PointWave &pointWave);

    /**
     * Adds the directional wave.
     * @param directionalWave
     */
    void addDirectionalWave(const DirectionalWave & directionalWave);

    /**
     * @param index
     * @returns the directional wave at the index.
     */
    DirectionalWave* getDirectionalWave(int index);

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
     * @returns the resulting displacement of the waves.
     */
    [[nodiscard]] glm::vec3 computeResultingPosition(float absoluteTime, glm::vec3 initialPosition) const;

    /**
     * @param absoluteTime
     * @param initialPosition of the objects (without any water displacement)
     * @returns the resulting orientation from the waves.
     */
    [[nodiscard]] glm::vec3 computeResultingNormal(float absoluteTime, glm::vec3 initialPosition) const;

    /**
     * Draws the water surface.
     * @param shader
     */
    void draw(const Shader & shader) const override;
};


#endif
