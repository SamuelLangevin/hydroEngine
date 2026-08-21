
#include "SceneRenderer.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "draw/Rectangle.hpp"
#include "draw/Sphere.hpp"
#include "utility/ResourceManager.hpp"
#include <GLFW/glfw3.h>
#include "utility/Waves.hpp"


void SceneRenderer::free() {
    glDeleteBuffers(1, &matricesUBO);
    Rectangle::free();
    ResourceManager::clear();
    delete water;
}

void SceneRenderer::init(glm::ivec2 windowSize) {
    loadShaders();
    setMatrixBlocks();
    initializeScene();
}

void SceneRenderer::loadShaders() {
    ResourceManager::loadShader("screenWaterShader", "screen.vert", "screenWater.frag");
    ResourceManager::loadShader("waterSurfaceShader", "waterSurface.vert", "waterSurface.frag",
                                    nullptr, "waterSurface.tesc", "waterSurface.tese");
    ResourceManager::loadShader("monoColorShader", "object.vert", "monoColor.frag");
}

void SceneRenderer::setMatrixBlocks() {
    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2*sizeof(glm::mat4));

    ResourceManager::getShader("monoColorShader")->setUniformBlock("Matrices", 0);
    ResourceManager::getShader("waterSurfaceShader")->setUniformBlock("Matrices", 0);
}

void SceneRenderer::initializeScene() {
    Shader * waterSurfaceShader = ResourceManager::getShader("waterSurfaceShader");
    waterSurfaceShader->use();

    DirectionalWave dWave(glm::vec2(0.2f, 0.7f));
    dWave.setUniforms(waterSurfaceShader, 0);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.7, 0.8, 1.0, 1.0);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    Shader * waterSurfaceShader = ResourceManager::getShader("waterSurfaceShader");

    waterSurfaceShader->use();
    waterSurfaceShader->setFloat("time", static_cast<float>(glfwGetTime()));

    waterSurfaceShader->setInt("nbOfPointWaves", waves.size());
    for (int i = 0; i < std::min(static_cast<int>(waves.size()), 50); ++i) {
        waves.at(i).setUniforms(waterSurfaceShader, i);
    }
    water->draw(waterSurfaceShader);
}

void SceneRenderer::drawWorldCursor(const glm::vec3 worldCursorPos) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader * monoColorShader = ResourceManager::getShader("monoColorShader");
    monoColorShader->use();
    monoColorShader->setVec4("color", glm::vec4(0.0, 0.5, 1.0, 0.5));
    Sphere worldCursor;
    worldCursor.position = worldCursorPos;
    worldCursor.draw(monoColorShader);
}

