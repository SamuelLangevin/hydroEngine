#ifndef SCENE_REPOSITORY_H
#define SCENE_REPOSITORY_H

#include "../utility/Waves.hpp"
#include "../draw/Surface.hpp"


class SceneRepository {
public:
    SceneRepository() = delete;

    static Surface * water; /**< The water surface's mesh.*/
    static std::vector<DirectionalWave> directionalWaves;
    static std::vector<PointWave> pointWaves;
    static std::vector<BasicShape*> entities; /**< The scene's objects. */

    /**
     * Erases the directional wave at the index's position.
     * @param index
     */
    static void eraseDirWave(int index);

    /**
     * Deletes the allocated data.
     */
    static void free();

};


#endif
