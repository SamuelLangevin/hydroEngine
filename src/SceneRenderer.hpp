#ifndef SCENERENDERER_HPP
#define SCENERENDERER_HPP
#include <vec2.hpp>
#include <vector>

#include "utility/Camera.hpp"
#include "utility/Waves.hpp"

class SceneRenderer {
    public:

        SceneRenderer() = default;
        ~SceneRenderer() = default;

        void init(glm::ivec2 windowSize);
        void free();
        void draw(const Camera & camera, glm::ivec2 windowSize) const;

        std::vector<PointWave> waves;

    private:
        uint matricesUBO = 0;

};

#endif
