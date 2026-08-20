#include "Application.hpp"
#include <iostream>
#include <detail/type_quat.hpp>
#include <ext/quaternion_trigonometric.hpp>

#include "utility/ResourceManager.hpp"
#include "utility/Utility.hpp"

SceneRenderer Application::sceneRenderer;
Camera Application::camera;

Application::AppState Application::state = ACTIVE;
glm::ivec2 Application::windowSize = glm::ivec2(1080, 810);
glm::vec2 Application::lastMousePos = glm::vec2(windowSize.x/2,windowSize.y/2);
float Application::sensitivity = 0.002f;
bool Application::firsttime = true;
uint Application::keys[1024];
uint Application::keysProcessed[1024];
bool Application::leftMouseButtonProcessed = false;

Application::Application() {
    initializeWindow();
    sceneRenderer.init(windowSize);
}

Application::~Application(){
    sceneRenderer.free();
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(window, key_callback);
}

void Application::mouse_callback(GLFWwindow * window, double xpos, double ypos){

    if(firsttime){
        lastMousePos = glm::vec2(xpos, ypos);
        firsttime = false;
    }

    float xOffset = sensitivity * (xpos - lastMousePos.x);
    float yOffset = sensitivity * -(ypos - lastMousePos.y);
    
    lastMousePos = glm::vec2(xpos, ypos);

    glm::quat qYaw = glm::angleAxis(xOffset, glm::vec3(0, -1, 0));
    glm::quat qPitch = glm::angleAxis(yOffset, glm::cross(camera.front, camera.up));
    camera.front = glm::normalize(qYaw * qPitch * camera.front);

}



void Application::framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    glViewport(0,0, width, height);
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
    if (state == ACTIVE) {

        const float cameraSpeed = 3.0f * deltaTime;
        if(keys[GLFW_KEY_W]) camera.position += cameraSpeed * camera.front;
        if(keys[GLFW_KEY_S]) camera.position -= cameraSpeed * camera.front;
        if(keys[GLFW_KEY_A]) camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
        if(keys[GLFW_KEY_D]) camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }
        if(keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !leftMouseButtonProcessed) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        leftMouseButtonProcessed = true;

        sceneRenderer.waves.emplace_back(glm::vec2(x/windowSize.x, (windowSize.y - y)/windowSize.y), glfwGetTime());
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) leftMouseButtonProcessed = false;
}

bool Application::shouldWindowClose(){
    return !glfwWindowShouldClose(window);
}

void Application::draw(){
    float currentframe = glfwGetTime();
    deltatime = currentframe - lastFrame;
    lastFrame = currentframe;

    if (state == ACTIVE) {
        sceneRenderer.draw(camera, windowSize);
        updateWaves();
        
    } else if (state == MENU) {
        
    } else glfwSetWindowShouldClose(window, true);

    glfwSwapBuffers(window);
    processInput(deltatime);
    glfwPollEvents();
    Utility::glCheckError();
    //checkFPS(deltatime);
}

void Application::updateWaves() {
    if (!sceneRenderer.waves.empty())
    if (glfwGetTime() - sceneRenderer.waves.at(0).dropTime > PointWave::lifeTime)
        sceneRenderer.waves.erase(sceneRenderer.waves.begin());
}

void Application::checkFPS(float dt) {
    static int fCounter = 0;
    if(fCounter > 500) {
        std::cout << "FPS: " << 1 / dt << std::endl;
        fCounter = 0;
    } else {
        fCounter++;
    }
}