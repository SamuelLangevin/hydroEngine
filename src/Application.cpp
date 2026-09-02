#include "Application.hpp"
#include <iostream>
#include "utility/Utility.hpp"

glm::ivec2 Application::windowSize = glm::ivec2(1080, 810);
glm::vec2 Application::lastMousePos = glm::vec2(windowSize.x/2,windowSize.y/2);
uint Application::mouseButtons[3];
uint Application::mouseButtonsProcessed[3];
uint Application::keys[1024];
uint Application::keysProcessed[1024];

Application::Application() {
    initializeWindow();
    sceneRenderer.init(windowSize);
    sceneManager.init();
    GuiManager::init(window, sceneManager.getDirWavesPointer(), sceneManager.getPntWavesPointer());
    setState(MENU);
}

Application::~Application(){
    sceneManager.free();
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

            if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !mouseButtonsProcessed[GLFW_MOUSE_BUTTON_LEFT]) {
                sceneManager.produceWave(camera, glfwGetTime(), windowSize, GuiManager::getPointWaveParameters());

                mouseButtonsProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
            }

            if(keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE]) {
                setState(MENU);

                keysProcessed[GLFW_KEY_ESCAPE] = true;
            }

        } break;

        case MENU: {
            if(keys[GLFW_KEY_ESCAPE] && !keysProcessed[GLFW_KEY_ESCAPE]) {
                setState(ACTIVE);
                keysProcessed[GLFW_KEY_ESCAPE] = true;
            }
        } break;
    }
}

void Application::setState(AppState state) {
    isFirstMouseMvt = true;
    glfwSetInputMode(window, GLFW_CURSOR, (state == ACTIVE ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    appState = state;
    GuiManager::setCaptureInput(state);
}

void Application::processCameraMovement() {

    if(isFirstMouseMvt){
        activeMousePos = lastMousePos;
        isFirstMouseMvt = false;
    }
    float xOffset = sensitivity * (lastMousePos.x - activeMousePos.x);
    float yOffset = sensitivity * -(lastMousePos.y - activeMousePos.y);
    activeMousePos = lastMousePos;

    camera.rotate(glm::vec2(xOffset, yOffset));
    camera.move(deltaTime, keys[GLFW_KEY_W], keys[GLFW_KEY_S], keys[GLFW_KEY_A], keys[GLFW_KEY_D]);
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
    sceneManager.update(glfwGetTime(), windowSize, camera);

    sceneRenderer.draw(camera, windowSize, sceneManager.getScene());
    if (appState == ACTIVE) sceneRenderer.drawWorldCursor(sceneManager.getCursorWorldPos());

    GuiManager::draw();

    glfwSwapBuffers(window);
    Utility::glCheckError();
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