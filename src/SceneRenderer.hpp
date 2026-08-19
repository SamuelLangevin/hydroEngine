#ifndef SCENERENDERER_HPP
#define SCENERENDERER_HPP
#include <vec2.hpp>
#include "utility/Camera.hpp"

class SceneRenderer {
    public:

        SceneRenderer() = default;
        ~SceneRenderer();

        void init(glm::ivec2 windowSize);
        void draw(const Camera & camera, glm::ivec2 windowSize) const;

    private:
        uint matricesUBO;

};

#endif
