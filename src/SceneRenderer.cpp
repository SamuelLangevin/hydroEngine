
#include "SceneRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "draw/Rectangle.hpp"
#include "draw/Sphere.hpp"
#include "utility/ResourceManager.hpp"
#include <GLFW/glfw3.h>

#include "utility/Waves.hpp"

SceneRenderer::~SceneRenderer() {
    glDeleteBuffers(1, &matricesUBO);
}

void SceneRenderer::init(glm::ivec2 windowSize) {
    ResourceManager::loadShader("screenWaterShader", "screen.vert", "screenWater.frag");

    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2*sizeof(glm::mat4));
}

void SceneRenderer::draw(const Camera & camera, const glm::ivec2 windowSize) const {

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getProjectionMatrix(windowSize)));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getViewMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    Shader * screenWaterShader = ResourceManager::getShader("screenWaterShader");

    DirectionalWave dWave(glm::vec2(0.2f, 0.7f));
    screenWaterShader->use();
    dWave.setUniforms(screenWaterShader);
    screenWaterShader->setFloat("time", static_cast<float>(glfwGetTime()));
    Rectangle::draw2DQuad();
}

