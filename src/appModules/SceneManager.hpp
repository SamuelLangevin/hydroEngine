#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <vector>

#include "../draw/Surface.hpp"
#include "../draw/Shape.hpp"
#include "../utility/Waves.hpp"


/** \class SceneManager
 * Manages the scene entities (objects) and the waves.
 */
class SceneManager {
public:


    struct Scene {
        Surface * water = nullptr; /**< The water surface's mesh.*/
        std::vector<DirectionalWave> directionalWaves;
        std::vector<PointWave> pointWaves;
        std::vector<Shape> entities;
    };

    /** Fills the scene and creates the waterSurface mesh. */
    void init();

    /** Frees the allocated memory */
    void free();

    /** @returns a const reference to the scene's objects. */
    [[nodiscard]] const Scene & getScene() const;

    /** @returns the corresponding world position of the cursor. */
    [[nodiscard]] glm::vec3 getCursorWorldPos() const;

    /** @returns a pointer to the list of directional waves. */
    [[nodiscard]] std::vector<DirectionalWave>* getDirWavesPointer();

    /** @returns a pointer to the list of point waves. */
    [[nodiscard]] std::vector<PointWave>* getPntWavesPointer();

    /**
     * Deletes the unactive point waves and updates the worldCursorPos
     * @param time
     * @param windowSize
     * @param camera
     */
    void update(float time, glm::vec2 windowSize, const Camera & camera);

    /**
     * Creates a wave according to the worldCursorPos
     * @param camera
     * @param currentTime
     * @param windowSize
     * @param pointWaveParameters
     */
    void produceWave(const Camera & camera, float currentTime, glm::ivec2 windowSize, const PointWave & pointWaveParameters);

private:
    Scene scene; /**< Stores the entities, the waves and other meshes. */
    glm::vec3 worldCursorPos; /**< The corresponding world position of the cursor. */

    /**
     * Deletes the waves according to the lifetime.
     * @param time
     */
    void deleteDeadWaves(float time);

};


#endif
