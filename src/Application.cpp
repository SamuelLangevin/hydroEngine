#include "Application.hpp"

#include <iostream>
#include <detail/type_quat.hpp>
#include <ext/quaternion_trigonometric.hpp>

#include "utility/ResourceManager.hpp"
#include "utility/Utility.hpp"
#include "imgui.h"

SceneRenderer Application::sceneRenderer;
GuiManager Application::guiManager;
GLFWwindow* Application::window = nullptr;
Camera Application::camera;

Application::AppState Application::appState = ACTIVE;
glm::ivec2 Application::windowSize = glm::ivec2(1080, 810);
glm::vec2 Application::lastMousePos = glm::vec2(windowSize.x/2,windowSize.y/2);
float Application::sensitivity = 0.002f;
bool Application::isFirstMouseMvt = true;
uint Application::keys[1024];
uint Application::keysProcessed[1024];
bool Application::leftMouseButtonProcessed = false;
glm::vec3 Application::worldCursorPos;

Application::Application() {
    initializeWindow();
    guiManager.init(window);
    sceneRenderer.init(windowSize);
}

Application::~Application(){
    sceneRenderer.free();
    guiManager.free();
    glfwTerminate();
}

void Application::initializeWindow(){
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(windowSize.x, windowSize.y, "HydroEngine", nullptr, nullptr);
	if (window == nullptr){
		std::cout << "failed to create GLFW window" << "\n";
		glfwTerminate();
	}
    glfwMakeContextCurrent(window);

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Failed to initialize glad" << "\n";
    }

    glViewport(0,0, windowSize.x, windowSize.y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
}

void Application::mouse_callback(GLFWwindow * window, double xpos, double ypos){

    if (appState == ACTIVE) {

        if(isFirstMouseMvt){
            lastMousePos = glm::vec2(xpos, ypos);
            isFirstMouseMvt = false;
        }

        float xOffset = sensitivity * (xpos - lastMousePos.x);
        float yOffset = sensitivity * -(ypos - lastMousePos.y);

        lastMousePos = glm::vec2(xpos, ypos);

        glm::quat qYaw = glm::angleAxis(xOffset, glm::vec3(0, -1, 0));
        glm::quat qPitch = glm::angleAxis(yOffset, glm::cross(camera.front, camera.up));
        camera.front = glm::normalize(qYaw * qPitch * camera.front);
    }

}



void Application::framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    windowSize = glm::ivec2(width, height);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (0 <= key && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
            keysProcessed[key] = false;
        }
    }
}


void Application::processInput(float deltaTime){

    switch (appState) {

        case ACTIVE: {

            const float cameraSpeed = 3.0f * deltaTime;
            if(keys[GLFW_KEY_W]) camera.position += cameraSpeed * camera.front;
            if(keys[GLFW_KEY_S]) camera.position -= cameraSpeed * camera.front;
            if(keys[GLFW_KEY_A]) camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
            if(keys[GLFW_KEY_D]) camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;

            const glm::vec2 screenCenter = 0.5f * glm::vec2(windowSize);
            worldCursorPos = Utility::getClickPositionOnPlane(screenCenter, camera, sceneRenderer.water->position,
                                                            glm::vec3(0.0f, 1.0f, 0.0f), windowSize);

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !leftMouseButtonProcessed) {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                leftMouseButtonProcessed = true;

                PointWave wave(glm::vec2(worldCursorPos.x, worldCursorPos.z),
                    glfwGetTime(), guiManager.pointWave.waveLength, guiManager.pointWave.magnitude, guiManager.pointWave.speed);
                sceneRenderer.pointWaves.push_back(wave);
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) leftMouseButtonProcessed = false;

            if(keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE]) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                appState = MENU;
                guiManager.setCaptureInput(true);

                keysProcessed[GLFW_KEY_ESCAPE] = true;
            }

        } break;

        case MENU: {

            if(keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE]) {
                isFirstMouseMvt = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                appState = ACTIVE;
                guiManager.setCaptureInput(false);

                keysProcessed[GLFW_KEY_ESCAPE] = true;
            }
        } break;
    }
}

bool Application::shouldWindowClose() const {
    return !glfwWindowShouldClose(window);
}

void Application::processFrame(){
    const float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(deltaTime);
    glfwPollEvents();
    updateWaves();

    sceneRenderer.draw(camera, windowSize);
    sceneRenderer.drawWorldCursor(worldCursorPos);

    guiManager.draw();

    glfwSwapBuffers(window);
    Utility::glCheckError();
    //checkFPS(deltaTime);
}

void Application::updateWaves() {
    for (uint i = 0; i < sceneRenderer.pointWaves.size(); ++i){
        if (glfwGetTime() - sceneRenderer.pointWaves.at(i).dropTime > PointWave::lifeTime)
            sceneRenderer.pointWaves.erase(sceneRenderer.pointWaves.begin() + i);
        else break; // expired waves will always be at the beginning of the vector;
    }
    *sceneRenderer.directionalWave = guiManager.directionalWave;
    sceneRenderer.directionalWave->direction = glm::normalize(sceneRenderer.directionalWave->direction);
}

void Application::printFPS(float dt) {
    static int fCounter = 0;
    if(fCounter > 500) {
        std::cout << "FPS: " << 1 / dt << std::endl;
        fCounter = 0;
    } else {
        fCounter++;
    }
}