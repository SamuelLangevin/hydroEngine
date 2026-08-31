#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#include "SceneRenderer.hpp"
#include <GLFW/glfw3.h>

#include "GuiManager.hpp"

/** \class Application
 * Manages windowing and user interaction. Uses SceneRenderer and GUIManager submodules.
 */
class Application{
    public:
        /** \enum AppState
         * - ACTIVE - The application focuses on the scene.
         * - MENU - The application focuses on the interface.
         */
        enum AppState {ACTIVE, MENU};

        /** Initializes the GLFWwindow and the SceneRenderer. */
        Application();

        /** Frees the SceneRenderer and the GLFWwindow's resources. */
        ~Application();

        /**
         * Asks glfw if windowShouldClose as been set to true.
         * @returns !glfwWindowShouldClose(window)
         */
        [[nodiscard]] bool shouldWindowClose() const;

        /** Processes user input, updates app behavior, and renders a frame. */
        void processFrame();

    private:

        static glm::ivec2 windowSize; /**< AKA viewport size in pixels. */
        static glm::vec2 lastMousePos; /**< Mouse position recorded by glfw. */
        static uint mouseButtons[3]; /**< Is the indexed mouse button being pressed ?.*/
        static uint mouseButtonsProcessed[3]; /**< Has the indexed mouse button's initial press been processed ?.*/
        static uint keys[1024]; /**< Is the indexed key being pressed ?.*/
        static uint keysProcessed[1024]; /**< Has the indexed key's initial press been processed ?.*/

        GLFWwindow* window = nullptr; /**< The window object, making the bridge between the app and the user. */
        SceneRenderer sceneRenderer; /**< Renders the scene and manages the OpenGL state. */
        Camera camera; /**< First person camera point of view for rendering. */

        AppState appState = MENU; /**< The state the application is in. @see AppState enum. */
        glm::vec2 activeMousePos = glm::vec2(windowSize.x/2,windowSize.y/2); /**< Mouse position recorded in active state. */
        glm::vec3 worldCursorPos{}; /**< World position of the screen's center on the water surface. */
        float deltaTime{}; /**< Time passed between the previous frame and the current frame in seconds. */
        float lastFrame{}; /**< Time stamp of the previous frame. */
        float cameraSpeed = 9.0f; /**< Movement speed of the camera*/
        float sensitivity = 0.002f; /**< Mouse sensitivity for the camera's orientation. */
        bool isFirstMouseMvt = true; /**< Is the first mouse movement processed ? */

        PointWave pointWaveParameters = GuiManager::DEFAULT_POINT_WAVE; /**< The selected parameters to generate point waves.*/
        std::vector<PointWave> pointWaves; /**< The point waves applied on the water surface. */
        std::vector<DirectionalWave> directionalWaves; /**< The directional waves applied on the water surface. */


        /** Initializes the window, its OpenGL context and callbacks. */
        void initializeWindow();

        /** Processes the user's key inputs and mouse movements. */
        void processInput();

        /**
         * Sets the app's state and changes the app input focus
         * @param state
         */
        void setState(AppState state);

        /** Deletes expired waves according to their lifeTime and updates the directional wave's attributes. */
        void updateWaves();

        /** Processes the camera's orientation and position change. */
        void processCameraMovement();

        /** Creates a wave id the worldCursor points a valid position. */
        void produceWave();

        /** Keeps track of mouse movements and updates the camera's point of view. */
        static void mouse_callback(GLFWwindow * window, double xpos, double ypos);

        /** Keeps track of the window's size. */
        static void framebuffer_size_callback(GLFWwindow * window, int width, int height);

        /** Keeps track of key events. */
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

        /** keeps track of mouse inputs. */
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        /** Prints the frames-per-second count. */
        static void printFPS(float dt);
};

#endif