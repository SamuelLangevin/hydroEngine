#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vec3.hpp>
#include "Shader.hpp"

/** \class Light
 * Base light attributes and functions
 */
class Light {
    public:

        glm::vec3 color; /**< Color of the light. */

        /**
         * Base light constructor
         * @param color
         */
        explicit Light(const glm::vec3 color) : color(color) {};
        virtual ~Light() = default;

        /** Some light types may be observable in the scene.
         * @param shader used to display the light's solid body.
         */
        virtual void draw(Shader * shader) = 0;

        /** Every light needs to be sent to the shaders to calculate illumination.
         * @param shader to send the data to
         * @param lightIndex
         */
        virtual void setUniforms(Shader * shader, int lightIndex) = 0;
};

/** \class AmbientLight
 * A light reaching every surface without any attenuation.
 */
class AmbientLight : public Light{
    public:
        /**
        * Simple ambient light constructor
        * @param color
        */
        explicit AmbientLight(glm::vec3 color);

        /** An ambient cannot be seen by itself. This function has no effect. */
        void draw(Shader * shader) override;

        /** Sends the light's color to the shader for lighting calculations.
        * @param shader to send the data for computing
        * @param lightIndex index of the light in its designated array
        */
        void setUniforms(Shader * shader, int lightIndex) override;
};

/** \class DirectionalLight
 * A sun like light source.
 */
class DirectionalLight : public Light{
    public:

        glm::vec3 direction; /**< The direction the light illuminates in. */

        /** Default directional light : white, points down*/
        DirectionalLight();

        /**
         * Non default constructor.
         * @param color
         * @param direction
         */
        DirectionalLight(glm::vec3 color, glm::vec3 direction);


        /** An ambient cannot be seen by itself. This function has no effect. */
        void draw(Shader * shader) override;

        /**
         * Sends the light's attributes to the shader.
         *
         * Assumes the DirectionalLight array uniform is named dirLights
         * and that their attributes are named identical.
         * @param shader to send the data for computing
         * @param lightIndex index of the light in its designated array
         */
        void setUniforms(Shader * shader, int lightIndex) override;

};


/** \class PointLight
 * Omnidirectional light source.
 */
class PointLight : public Light{
    public:
    
        glm::vec3 position; /**< The light's solid body's position. */
        float strength = 32.0f; /**< Linear range factor of the light's effect. */

        /**
         * Default point light : white, positioned at the origin
         */
        PointLight();

        /**
         * Non default constructor.
         * @param color
         * @param position
         */
        PointLight(glm::vec3 color, glm::vec3 position);

        /**
         * Draws the light as a sphere in the scene.
         * @param shader used to draw the light's solid body.
         */
        void draw(Shader * shader) override;

        /**
         * Sends the light's attributes to the shader.
         *
         * Assumes the SpotLight array uniform is named spotLights
         * and that their attributes are named identical.
         * @param shader to send the data for computing
         * @param lightIndex index of the light in its designated array
         */
        void setUniforms(Shader * shader, int lightIndex) override;
};


/** \class
 * A flashlight like light source.
 */
class SpotLight : public Light{
    public:
        glm::vec3 position; /**< The light's solid body's position. */
        glm::vec3 direction; /**< The direction the light illuminates in. */
        float strength = 32.0f; /**< Linear range factor of the light's effect. */
        float angularAttenuation = 5.0f; /**< Angle at which point the light no longer illuminates. */

        /** Default spot light : white, positioned at the origin and points down*/
        SpotLight();

        /**
         * Non default constructor.
         * @param color
         * @param position
         * @param direction
         */
        SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 direction);

        /**
         * Draws the light as a cone in the scene.
         * @param shader
         */
        void draw(Shader * shader) override;

        /**
         * Sends the light's color, position, to the shader.
         * @param shader to send the data for computing
         * @param lightIndex index of the light in its designated array
         */
        void setUniforms(Shader * shader, int lightIndex) override;

        /**
         * Used to orientate the cone's base towards the direction it illuminates.
         * @returns orientation quat in world space
         */
        [[nodiscard]] glm::quat getLookAtQuat() const;
};


#endif
