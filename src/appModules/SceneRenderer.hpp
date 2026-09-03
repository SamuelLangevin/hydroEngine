#ifndef SCENE_RENDERER_HPP
#define SCENE_RENDERER_HPP
#include <vec2.hpp>

#include "../draw/Surface.hpp"
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
         * Loads shaders, textures and framebuffers, creates the scene's objects and sets their corresponding uniforms.
         * @param windowSize AKA viewport
         */
        void init(glm::ivec2 windowSize);

        /**
         * Frees the ResourceRepository's resources, scene objects data and OpenGL resources.
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
         * Creates an irradiance (or diffuse light) map of the skybox for light calculations.
         * Optionally saves it as an image to avoid generating it every startup.
         * Based on https://learnopengl.com/PBR/IBL/Diffuse-irradiance
         * @param captureProjection projection matrix
         * @param captureView view matrix
         * @param saveAsImage option to save the cube map as images.
         */
        static void createEnvIrradianceCubemap(const glm::mat4 & captureProjection, const glm::mat4 * captureView, bool saveAsImage);

        /**
         * Creates the pre-filtered environment map of the skybox for specular light calculations.
         * Gives for a combination of reflection direction and roughness what the environment looks like from the light's dispersed reflection.
         * Based on https://learnopengl.com/PBR/IBL/Specular-IBL
         * @param captureProjection projection matrix
         * @param captureView view matrix
         *
         * todo make it possible to save each mipmap level as one image (in a cross layout)
         */
        static void createPrefilteredMipMaps(const glm::mat4 & captureProjection, const glm::mat4 * captureView);

        /**
         * Creates the BRDF integration map (or LUT texture) for specular light calculations.
         * Gives for a combination of roughness and viewing angle, how the environment contribution is scaled according to the microfacet BRDF.
         * Optionally saves it an image to avoid generating it every startup.
         * Based on https://learnopengl.com/PBR/IBL/Specular-IBL
         * @param saveAsImage option to save this texture as an image.
         */
        static void createLUTTexture(bool saveAsImage);
};

#endif
