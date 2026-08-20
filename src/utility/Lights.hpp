#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vec3.hpp>
#include "Shader.hpp"

class Light {
    public:

        glm::vec3 color;

        Light(const glm::vec3 color) : color(color) {};
        virtual ~Light() = default;
        virtual void draw(Shader * shader) = 0;
        virtual void setUniforms(Shader * shader, int lightIndex) = 0;
};



class AmbientLight : public Light{
    public:

        AmbientLight(glm::vec3 color);

        void draw(Shader * shader) override;
        void setUniforms(Shader * shader, int lightIndex) override;
};



class DirectionalLight : public Light{
    public:

        glm::vec3 direction;

        DirectionalLight();
        DirectionalLight(glm::vec3 color, glm::vec3 direction);

        void draw(Shader * shader) override;
        void setUniforms(Shader * shader, int lightIndex) override;

};



class PointLight : public Light{
    public:
    
        glm::vec3 position;
        float strength = 32.0f;

        PointLight();
        PointLight(glm::vec3 color, glm::vec3 position);

        void draw(Shader * shader) override;
        void setUniforms(Shader * shader, int lightIndex) override;
};



class SpotLight : public Light{
    public:
        glm::vec3 position;
        glm::vec3 direction;
        float strength = 32.0f;
        float angularAttenuation = 5.0f;

        SpotLight();
        SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 direction);

        void draw(Shader * shader) override;

        void setUniforms(Shader * shader, int lightIndex) override;

        glm::quat getLookAtQuat();
};


#endif
