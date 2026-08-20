
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
}

void SceneRenderer::init(glm::ivec2 windowSize) {
    ResourceManager::loadShader("screenWaterShader", "screen.vert", "screenWater.frag");

    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2*sizeof(glm::mat4));

    Shader * screenWaterShader = ResourceManager::getShader("screenWaterShader");
    screenWaterShader->use();

    DirectionalWave dWave(glm::vec2(0.2f, 0.7f));
    dWave.setUniforms(screenWaterShader, 0);
}

void SceneRenderer::draw(const Camera & camera, const glm::ivec2 windowSize) const {

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getProjectionMatrix(windowSize)));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getViewMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    Shader * screenWaterShader = ResourceManager::getShader("screenWaterShader");

    screenWaterShader->use();
    screenWaterShader->setFloat("time", static_cast<float>(glfwGetTime()));


    screenWaterShader->setInt("nbOfPointWaves", waves.size());
    for (int i = 0; i < std::min(static_cast<int>(waves.size()), 50); ++i) {
        waves.at(i).setUniforms(screenWaterShader, i);
    }
    
    Rectangle::draw2DQuad();
}

