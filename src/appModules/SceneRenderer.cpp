
#include "SceneRenderer.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "../draw/Sphere.hpp"
#include "../draw/Cube.hpp"
#include "../utility/ResourceManager.hpp"
#include <GLFW/glfw3.h>

#include "../draw/Rectangle.hpp"
#include "../utility/Utility.hpp"
#include "../utility/Waves.hpp"
#include "../../includes/stb_image.h"


void SceneRenderer::free() {
    glDeleteBuffers(1, &matricesUBO);
    Sphere::free();
    ResourceManager::clear();
    Rectangle::free();
    Cube::free();
}

void SceneRenderer::init(glm::ivec2 windowSize) {
    loadShaders();
    setUniformBlocks();
    loadTextures();
    initializeScene();
}

void SceneRenderer::loadTextures() {
    using RM =  ResourceManager;

    RM::addTexture("lakeSkybox", Texture::cubemapFromDirectory("../resources/textures/cubemaps/lake/"),
        Texture::lastCreatedImageSize, GL_TEXTURE_CUBE_MAP);
    stbi_set_flip_vertically_on_load(true);
    RM::addTexture("lakeIrradianceMap", Texture::cubemapFromDirectory("../resources/textures/cubemaps/lake_IrradianceMap/"),
        Texture::lastCreatedImageSize, GL_TEXTURE_CUBE_MAP);
    RM::addTexture("lutTexture", Texture::textureFromFile("LUTTexture.png", "../resources/textures/"),
        Texture::lastCreatedImageSize, GL_TEXTURE_2D);
    stbi_set_flip_vertically_on_load(false);
    ResourceManager::addTexture("deepBlue", Texture::createColorTexture(glm::vec3(0.0f, 0.05f, 0.1f)), glm::ivec2(1), GL_TEXTURE_2D);
    ResourceManager::addTexture("white", Texture::createColorTexture(glm::vec3(1.0f)), glm::ivec2(1), GL_TEXTURE_2D);
    ResourceManager::addTexture("red", Texture::createColorTexture(glm::vec3(1.0f, 0.0f, 0.0f)), glm::ivec2(1), GL_TEXTURE_2D);

    createIBLTextures();
}


void SceneRenderer::loadShaders() {
    using RM =  ResourceManager;
    RM::addShader("screenWaterShader", Shader::createShader("screen.vert", "screenWater.frag"));
    RM::addShader("waterSurfaceShader", Shader::createShader("waterSurface.vert",
        "pbr.frag", nullptr, "waterSurface.tesc", "waterSurface.tese"));
    RM::addShader("monoColorShader", Shader::createShader("object.vert", "monoColor.frag"));
    RM::addShader("skyboxShader", Shader::createShader("cubemap.vert", "cubemap.frag"));
    RM::addShader("object", Shader::createShader("object.vert", "pbr.frag"));
}

void SceneRenderer::setUniformBlocks() {
    glGenBuffers(1, &matricesUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2*sizeof(glm::mat4));

    ResourceManager::getShader("monoColorShader").setUniformBlock("Matrices", 0);
    ResourceManager::getShader("waterSurfaceShader").setUniformBlock("Matrices", 0);
    ResourceManager::getShader("skyboxShader").setUniformBlock("Matrices", 0);
    ResourceManager::getShader("object").setUniformBlock("Matrices", 0);
}

void SceneRenderer::initializeScene() {
    Shader waterSurfaceShader = ResourceManager::getShader("waterSurfaceShader");
    waterSurfaceShader.use();
    ResourceManager::getTexture("lakeIrradianceMap").bind(waterSurfaceShader, "environment.irradianceMap",1);
    ResourceManager::getTexture("prefilterMap").bind(waterSurfaceShader, "environment.prefilterMap",2);
    ResourceManager::getTexture("lutTexture").bind(waterSurfaceShader, "environment.brdfLUT",3);

    Shader objectShader = ResourceManager::getShader("object");
    objectShader.use();
    ResourceManager::getTexture("lakeIrradianceMap").bind(objectShader, "environment.irradianceMap",1);
    ResourceManager::getTexture("prefilterMap").bind(objectShader, "environment.prefilterMap",2);
    ResourceManager::getTexture("lutTexture").bind(objectShader, "environment.brdfLUT",3);
}

void SceneRenderer::createIBLTextures() {
    Utility::FrameBuffer envCubemapFBO;
    Utility::createFrameBuffer(envCubemapFBO, glm::ivec2(512), true, 0);

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureView[] = {
        glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
    };

    glBindFramebuffer(GL_FRAMEBUFFER, envCubemapFBO.ID);
        glBindRenderbuffer(GL_RENDERBUFFER, envCubemapFBO.renderBuffer);
        //createEnvIrradianceCubemap(captureProjection, &captureView[0], false); // has been pre-generated
        createPrefilteredMipMaps(captureProjection, &captureView[0]);
        //createLUTTexture(false); // has been pre-generated
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDeleteFramebuffers(1, &envCubemapFBO.ID);
    glDeleteRenderbuffers(1, &envCubemapFBO.renderBuffer);
}

void SceneRenderer::createEnvIrradianceCubemap(const glm::mat4 & captureProjection, const glm::mat4 * captureView, bool saveAsImage){
    constexpr glm::ivec2 IRRADIANCE_TEX_SIZE(32);
    uint envIrradianceTexture = Texture::createCubemapTexture(IRRADIANCE_TEX_SIZE);
    ResourceManager::addTexture("envIrradianceTexture", envIrradianceTexture,
                                IRRADIANCE_TEX_SIZE, GL_TEXTURE_CUBE_MAP);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, IRRADIANCE_TEX_SIZE.x, IRRADIANCE_TEX_SIZE.y);

    Shader cubemapConvolutionShader = Shader(Shader::createShader("pbr/position.vert", "pbr/cubemapConvolution.frag"));
    cubemapConvolutionShader.use();
    cubemapConvolutionShader.setInt("environmentMap", 0);
    cubemapConvolutionShader.setMat4("projection", captureProjection);
    ResourceManager::getTexture("lakeSkybox").bind(cubemapConvolutionShader, "environmentMap", 0);

    glViewport(0, 0, IRRADIANCE_TEX_SIZE.x, IRRADIANCE_TEX_SIZE.y);
    for (uint i = 0; i < 6; i++) {
        cubemapConvolutionShader.setMat4("view", captureView[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envIrradianceTexture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Cube::draw();
        if (saveAsImage) Texture::saveTextureToFile(std::to_string(i) + "_irradianceCubemap.tga", IRRADIANCE_TEX_SIZE);
    }
    cubemapConvolutionShader.free();
}

void SceneRenderer::createPrefilteredMipMaps(const glm::mat4 & captureProjection, const glm::mat4 * captureView) {
    constexpr glm::ivec2 MIPMAPS_SIZE(128);
    uint prefilterMap = Texture::createCubemapTexture(MIPMAPS_SIZE);
    ResourceManager::addTexture("prefilterMap", prefilterMap, MIPMAPS_SIZE,  GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    Shader prefilterConvolutionShader = Shader(Shader::createShader("pbr/position.vert", "pbr/prefilterConvolution.frag"));
    prefilterConvolutionShader.use();
    prefilterConvolutionShader.setMat4("projection", captureProjection);
    ResourceManager::getTexture("lakeSkybox").bind(prefilterConvolutionShader, "environmentMap", 0);

    uint maxMipLevels = 5;
    for (uint mipLevel = 0; mipLevel < maxMipLevels; mipLevel++) {
        glm::ivec2 resolution =  glm::ivec2(static_cast<float>(MIPMAPS_SIZE.x) * std::pow(0.5, mipLevel));
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution.x, resolution.y);
        glViewport(0, 0, resolution.x, resolution.y);
        float roughness = (float) mipLevel / (float)(maxMipLevels-1);
        prefilterConvolutionShader.setFloat("roughness", roughness);
        for (uint i = 0; i < 6; i++) {
            prefilterConvolutionShader.setMat4("view", captureView[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mipLevel);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Cube::draw();
        }
    }
    prefilterConvolutionShader.free();
}

void SceneRenderer::createLUTTexture(bool saveAsImage) {
    constexpr glm::ivec2 LUT_TEX_SIZE(512);
    uint brdfLUTTexture = Texture::createTexture(LUT_TEX_SIZE, GL_RG16F, GL_RG, GL_FLOAT, nullptr);
    ResourceManager::addTexture("lutTexture", brdfLUTTexture, LUT_TEX_SIZE, GL_TEXTURE_2D);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, LUT_TEX_SIZE.x, LUT_TEX_SIZE.y);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
    glViewport(0, 0, LUT_TEX_SIZE.x, LUT_TEX_SIZE.y);
    Shader brdfConvolutionShader = Shader(Shader::createShader("pbr/BRDFConvolution.vert", "pbr/BRDFConvolution.frag"));
    brdfConvolutionShader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Rectangle::draw2DQuad();
    if (saveAsImage) Texture::saveTextureToFile("LUTTexture", LUT_TEX_SIZE);
    brdfConvolutionShader.free();

}

void SceneRenderer::draw(const Camera & camera, const glm::ivec2 windowSize, const SceneManager::Scene & scene) const {
    using RM = ResourceManager;

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getProjectionMatrix(windowSize)));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getViewMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.7, 0.8, 1.0, 1.0);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    Shader waterSurfaceShader = RM::getShader("waterSurfaceShader");
    Shader objectShader = RM::getShader("object");

    waterSurfaceShader.use();
    waterSurfaceShader.setFloat("time", static_cast<float>(glfwGetTime()));
    waterSurfaceShader.setVec3("viewPos", camera.getPosition());

    waterSurfaceShader.setInt("nbOfPointWaves", scene.pointWaves.size());
    for (int i = 0; i < std::min(static_cast<int>(scene.pointWaves.size()), 50); ++i) {
        scene.pointWaves.at(i).setUniforms(&waterSurfaceShader, "pointWaves[" + std::to_string(i) + "]");
    }
    waterSurfaceShader.setInt("nbOfDirectionalWaves", scene.directionalWaves.size());
    for (int i = 0; i < std::min(static_cast<int>(scene.directionalWaves.size()), 50); ++i) {
        scene.directionalWaves.at(i).setUniforms(&waterSurfaceShader, "directionalWaves[" + std::to_string(i) + "]");
    }

    scene.water->setUniforms(waterSurfaceShader, 0);
    scene.water->draw(waterSurfaceShader);

    objectShader.use();
    objectShader.setVec3("viewPos", camera.getPosition());
    for (auto entity: scene.entities) {
        entity->setUniforms(objectShader, 0);
        entity->draw(objectShader);
    }

    Shader skyboxShader = RM::getShader("skyboxShader");
    Cube::drawSkyBox(skyboxShader, RM::getTexture("lakeSkybox"), "skybox");
}

void SceneRenderer::drawWorldCursor(const glm::vec3 worldCursorPos) const {
    Shader monoColorShader = ResourceManager::getShader("monoColorShader");
    monoColorShader.use();
    monoColorShader.setVec4("color", glm::vec4(0.0, 0.5, 1.0, 0.5));

    Sphere worldCursor;
    worldCursor.position = worldCursorPos + glm::vec3(0.0, 0.0, 0.0);
    worldCursor.draw(monoColorShader);
}

