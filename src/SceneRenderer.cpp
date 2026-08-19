
#include "SceneRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "draw/Sphere.hpp"
#include "utility/ResourceManager.hpp"

SceneRenderer::~SceneRenderer() {

}

void SceneRenderer::init(glm::ivec2 windowSize) {
    ResourceManager::loadShader("testShader", "test.vert", "test.frag");
    Shader * testShader = ResourceManager::getShader("testShader");
    testShader->setUniformBlock("Matrices", 0);

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    Shader * testShader = ResourceManager::getShader("testShader");
    testShader->use();
    Sphere sphere;
    sphere.draw(testShader);

}