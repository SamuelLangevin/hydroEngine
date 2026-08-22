
#include "SceneRenderer.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "draw/Sphere.hpp"
#include "utility/ResourceManager.hpp"
#include <GLFW/glfw3.h>
#include "utility/Waves.hpp"


void SceneRenderer::free() {
    glDeleteBuffers(1, &matricesUBO);
    Sphere::free();
    ResourceManager::clear();
    delete water;
    delete directionalWave;
}

void SceneRenderer::init(glm::ivec2 windowSize) {
    loadShaders();
    setUniformBlocks();
    initializeScene();
}

void SceneRenderer::loadShaders() {
    ResourceManager::addShader("screenWaterShader", Shader::createShader("screen.vert", "screenWater.frag"));
    ResourceManager::addShader("waterSurfaceShader", Shader::createShader("waterSurface.vert", "waterSurface.frag",
                                    nullptr, "waterSurface.tesc", "waterSurface.tese"));
    ResourceManager::addShader("monoColorShader", Shader::createShader("object.vert", "monoColor.frag"));
}

void SceneRenderer::setUniformBlocks() {
    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2*sizeof(glm::mat4));

    ResourceManager::getShader("monoColorShader").setUniformBlock("Matrices", 0);
    ResourceManager::getShader("waterSurfaceShader").setUniformBlock("Matrices", 0);
}

void SceneRenderer::initializeScene() {
    Shader waterSurfaceShader = ResourceManager::getShader("waterSurfaceShader");
    waterSurfaceShader.use();

    directionalWave = new DirectionalWave(glm::vec2(0.2f, 0.7f));
    directionalWave->setUniforms(&waterSurfaceShader, "dirWave");

    water = new Surface(glm::ivec2(1000));
    water->scale = glm::vec3(0.1f);
    water->position = glm::vec3(0.0f, -10.0f, 0.0f);
}

void SceneRenderer::draw(const Camera & camera, const glm::ivec2 windowSize) const {

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getProjectionMatrix(windowSize)));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getViewMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.7, 0.8, 1.0, 1.0);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    Shader waterSurfaceShader = ResourceManager::getShader("waterSurfaceShader");

    waterSurfaceShader.use();
    waterSurfaceShader.setFloat("time", static_cast<float>(glfwGetTime()));

    waterSurfaceShader.setInt("nbOfPointWaves", pointWaves.size());
    for (int i = 0; i < std::min(static_cast<int>(pointWaves.size()), 50); ++i) {
        pointWaves.at(i).setUniforms(&waterSurfaceShader, "pointWaves[" + std::to_string(i) + "]");
    }
    water->draw(&waterSurfaceShader);
}

void SceneRenderer::drawWorldCursor(const glm::vec3 worldCursorPos) const {
    Shader monoColorShader = ResourceManager::getShader("monoColorShader");
    monoColorShader.use();
    monoColorShader.setVec4("color", glm::vec4(0.0, 0.5, 1.0, 0.5));

    Sphere worldCursor;
    worldCursor.position = worldCursorPos + glm::vec3(0.0, directionalWave->magnitude, 0.0);
    worldCursor.draw(&monoColorShader);
}

