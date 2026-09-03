#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <vector>

#include "../draw/Surface.hpp"
#include "../draw/Shape.hpp"
#include "../scene/Waves.hpp"


/** \class SceneManager
 * Manages the scene entities (objects) and the waves.
 */
class SceneManager {
public:

    /** Fills the scene and creates the waterSurface mesh. */
    void init();

    /** Frees the allocated memory */
    void free();

    /** @returns the corresponding world position of the cursor. */
    [[nodiscard]] glm::vec3 getCursorWorldPos() const;

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
    glm::vec3 worldCursorPos{}; /**< The corresponding world position of the cursor. */

    /**
     * Deletes the waves according to the lifetime.
     * @param time
     */
    void deleteDeadWaves(float time);

    /**
     * Displaces the scene's entities according to the waves and the time.
     * @param time absolute time
     */
    void displaceObjects(float time);

    static glm::quat getLookAtQuat(glm::vec3 direction);

};


#endif
