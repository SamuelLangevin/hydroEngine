#ifndef SCENE_RENDERER_HPP
#define SCENE_RENDERER_HPP
#include <vec2.hpp>

#include "../scene/Camera.hpp"
#include "../resources/Utility.hpp"

/** \class SceneRenderer
 * Renders the scene view and non-UI elements.
 */
class SceneRenderer {
    public:

        SceneRenderer() = default;
        ~SceneRenderer() = default;

        /**
         * Loads shaders, textures and framebuffers and sets their corresponding uniforms.
         */
        void init();

        /**
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

    private:
        uint matricesUBO = 0; /**< The view and projection matrices uniform block's ID. */

        /** Loads the shaders to the ResourceRepository. */
        static void loadShaders();

        /** Generates and sets the uniform blocks used by the shaders. */
        void setUniformBlocks();

        /** Creates the scene's objects and sets their uniforms to the shaders they are rendered with. */
        void initializeScene();

        /** Loads the needed textures to the ResourceRepository. */
        static void loadTextures();

        /**
         * Creates the necessary textures for image-based lighting (IBL).
         */
        static void createIBLTextures();

        /**
         * Creates the pre-filtered environment map of the skybox for specular light calculations.
         * Gives for a combination of reflection direction and roughness what the environment looks like from the light's dispersed reflection.
         * Based on https://learnopengl.com/PBR/IBL/Specular-IBL
         * @param captureProjection projection matrix
         * @param captureView view matrix
         */
        static void createPrefilteredMipMaps(const glm::mat4 & captureProjection, const glm::mat4 * captureView);
};

#endif
