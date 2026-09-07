#ifndef SCENE_REPOSITORY_H
#define SCENE_REPOSITORY_H

#include <memory>

#include "../scene/Waves.hpp"
#include "../scene/WaterSurface.hpp"
#include "../scene/Entity.hpp"


/**
 * Stores the scene's objects.
 */
class SceneRepository {
private:
    static std::shared_ptr<WaterSurface> water; /**< The water surface's mesh.*/
    static std::vector<std::shared_ptr<Entity>> entities; /**< The scene's objects. */

public:
    SceneRepository() = delete;

    /** Creates the waterSurface. */
    static void initWaterSurface();

    /** @returns a pointer to the waterSurface.*/
    static std::shared_ptr<WaterSurface> getWaterSurface();

    /**
     * Adds the entity to the scene.
     * fixme WARNING : it assumes it has been allocated on the heap.
     * @param entity
     */
    static void addEntity(const std::shared_ptr<Entity>& entity);

    /**
     * @param index
     * @returns the entity at the index's position.
     */
    static std::shared_ptr<Entity> getEntity(int index);

    /**
     * Draws all the stored entities with the passed shader.
     * @param shader
     */
    static void drawEntities(const Shader & shader);

    /**
     * Deletes the allocated data.
     */
    static void free();

    /** @returns the number of stored entities.*/
    static int getNbOfEntities();
};


#endif
