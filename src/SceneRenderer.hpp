#ifndef SCENE_RENDERER_HPP
#define SCENE_RENDERER_HPP
#include <vec2.hpp>
#include <vector>

#include "draw/Surface.hpp"
#include "utility/Camera.hpp"
#include "utility/Waves.hpp"

/** \class SceneRenderer
 * Renders the scene view and non-UI elements.
 */
class SceneRenderer {
    public:

        SceneRenderer() = default;
        ~SceneRenderer() = default;

        /**
         * Loads shaders, textures and framebuffers, creates the scene's objects and sets their corresponding uniforms.
         * @param windowSize AKA viewport
         */
        void init(glm::ivec2 windowSize);

        /**
         * Frees the ResourceManager's resources, scene objects data and OpenGL resources.
         * Must be called before glfwTerminate().
         * @see ResourceManager.
         */
        void free();

        /**
         * Renders a frame of the scene according to the camera's view and the windowSize
         * @param camera
         * @param windowSize AKA viewport
         */
        void draw(const Camera & camera, glm::ivec2 windowSize) const;

        /**
         * Draws a world cursor (sphere) at the specified position.
         * @param worldCursorPos
         */
        void drawWorldCursor(glm::vec3 worldCursorPos) const;

        Surface * water = nullptr; /**< The water surface's mesh.*/
        DirectionalWave * directionalWave = nullptr; /**< The wave front applied on the water surface. */
        std::vector<PointWave> pointWaves; /**< The point waves applied on the water surface. */

    private:
        uint matricesUBO = 0; /**< The view and projection matrices uniform block's ID. */

        /** Loads the shaders to the ResourceManager. */
        static void loadShaders();

        /** Generates and sets the uniform blocks used by the shaders. */
        void setUniformBlocks();

        /** Creates the scene's objects and sets their uniforms to the shaders they are rendered with. */
        void initializeScene();

};

#endif
