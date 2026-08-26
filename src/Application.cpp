#include "Application.hpp"

#include <iostream>
#include <detail/type_quat.hpp>
#include <ext/quaternion_trigonometric.hpp>

#include "utility/ResourceManager.hpp"
#include "utility/Utility.hpp"
#include "imgui.h"

glm::ivec2 Application::windowSize = glm::ivec2(1080, 810);
glm::vec2 Application::lastMousePos = glm::vec2(windowSize.x/2,windowSize.y/2);
uint Application::mouseButtons[3];
uint Application::mouseButtonsProcessed[3];
uint Application::keys[1024];
uint Application::keysProcessed[1024];

Application::Application() {
    initializeWindow();
    GuiManager::init(window);
    sceneRenderer.init(windowSize);
}

Application::~Application(){
    sceneRenderer.free();
    GuiManager::free();
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
}

void Application::mouse_callback(GLFWwindow * window, double xpos, double ypos) {
    lastMousePos = glm::vec2(xpos, ypos);
}



void Application::framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    windowSize = glm::ivec2(width, height);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (0 <= button && button < 3)
    {
        if (action == GLFW_PRESS) mouseButtons[button] = true;
        else if (action == GLFW_RELEASE)
        {
            mouseButtons[button] = false;
            mouseButtonsProcessed[button] = false;
        }
    }
}


void Application::processInput(){

    switch (appState) {

        case ACTIVE: {
            processCameraMovement();

            worldCursorPos = Utility::getClickPositionOnPlane(0.5f * glm::vec2(windowSize), camera,
                sceneRenderer.water->position, glm::vec3(0.0f, 1.0f, 0.0f), windowSize);

            if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !mouseButtonsProcessed[GLFW_MOUSE_BUTTON_LEFT]) {
                produceWave();

                mouseButtonsProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
            }

            if(keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE]) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                appState = MENU;
                GuiManager::setCaptureInput(true);

                keysProcessed[GLFW_KEY_ESCAPE] = true;
            }

        } break;

        case MENU: {
            if(keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE]) {
                isFirstMouseMvt = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                appState = ACTIVE;
                GuiManager::setCaptureInput(false);

                keysProcessed[GLFW_KEY_ESCAPE] = true;
            }
        } break;
    }
}

void Application::processCameraMovement() {

    if(isFirstMouseMvt){
        activeMousePos = lastMousePos;
        isFirstMouseMvt = false;
    }

    float xOffset = sensitivity * (lastMousePos.x - activeMousePos.x);
    float yOffset = sensitivity * -(lastMousePos.y - activeMousePos.y);

    activeMousePos = lastMousePos;

    glm::quat qYaw = glm::angleAxis(xOffset, glm::vec3(0, -1, 0));
    glm::quat qPitch = glm::angleAxis(yOffset, glm::cross(camera.front, camera.up));
    camera.front = glm::normalize(qYaw * qPitch * camera.front);

    float cameraAdvance = cameraSpeed * deltaTime;
    if (keys[GLFW_KEY_W]) camera.position += cameraAdvance * camera.front;
    if (keys[GLFW_KEY_S]) camera.position -= cameraAdvance * camera.front;
    if (keys[GLFW_KEY_A]) camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraAdvance;
    if (keys[GLFW_KEY_D]) camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraAdvance;
}

void Application::produceWave() {
    const glm::vec2 screenCenter = 0.5f * glm::vec2(windowSize);
    glm::vec3 nearClipClick = camera.screenClickToNearClip(screenCenter, windowSize);
    if (glm::dot(camera.position - nearClipClick, camera.position - worldCursorPos) > 0.0f) {
        PointWave wave(glm::vec2(worldCursorPos.x, worldCursorPos.z), glfwGetTime(),
        GuiManager::pointWave.getWaveLength(), GuiManager::pointWave.getAmplitude(), GuiManager::pointWave.getSpeed());
        sceneRenderer.pointWaves.push_back(wave);
    }
}

bool Application::shouldWindowClose() const {
    return !glfwWindowShouldClose(window);
}

void Application::processFrame(){
    const float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwPollEvents();
    processInput();
    updateWaves();

    sceneRenderer.draw(camera, windowSize);
    if (appState == ACTIVE) sceneRenderer.drawWorldCursor(worldCursorPos);

    GuiManager::draw();

    glfwSwapBuffers(window);
    Utility::glCheckError();
    //checkFPS(deltaTime);
}

void Application::updateWaves() {
    if (GuiManager::resetWaves) sceneRenderer.pointWaves.clear();

    *sceneRenderer.directionalWave = GuiManager::directionalWave;

    for (uint i = 0; i < sceneRenderer.pointWaves.size(); ++i){
        const PointWave & wave = sceneRenderer.pointWaves[i];
        if (glfwGetTime() - wave.getDropTime() > wave.getLifetime())
            sceneRenderer.pointWaves.erase(sceneRenderer.pointWaves.begin() + i);
        else break;
    }
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