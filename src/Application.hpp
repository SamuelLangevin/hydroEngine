#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#include "SceneRenderer.hpp"
#include <GLFW/glfw3.h>

class Application{
    public:
        Application();
        ~Application();
        bool shouldWindowClose();
        void draw();

    private:

        enum AppState {ACTIVE, MENU};

        GLFWwindow* window;
        static SceneRenderer sceneRenderer;
        static Camera camera;
        static AppState state;

        static glm::ivec2 windowSize;
        static glm::vec2 lastMousePos;
        static float sensitivity;
        static bool firsttime;
        static uint keys[1024];
        static uint keysProcessed[1024];
        static bool leftMouseButtonProcessed;

        float deltatime = 0.0f;
        float lastFrame = 0.0f;

        void initializeWindow();
        void processInput(float deltaTime);
        void updateWaves();

        static void mouse_callback(GLFWwindow * window, double xpos, double ypos);
        static void framebuffer_size_callback(GLFWwindow * window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void checkFPS(float dt);
};

#endif