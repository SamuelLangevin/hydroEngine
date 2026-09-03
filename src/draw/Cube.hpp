#ifndef CUBE_H
#define CUBE_H

#include "../resources/Shader.hpp"
#include "BasicShape.hpp"

/** \class Cube
 * Allows to draw cubes and skyboxes.
 */
class Cube final : public BasicShape {
    public:

        Cube() = default;

        /** Binds the cube VAO. Generates it if necessary. */
        static void load();

        /** Draws a cube mesh without uniforms. */
        static void draw();

        /**< Draws the cube with its uniforms. */
        void draw(const Shader & shader) const override;

        /** Deletes the VAO, VBO and EBO */
        static void free();

        /**
         * Draws a skybox.
         * @param shader
         * @param skyBoxTex texture ID of the skybox
         * @param samplerName the name of the samplerCube uniform
         */
        static void drawSkyBox(const Shader & shader, const Texture & skyBoxTex, const std::string & samplerName);

private:
    static uint VAO; /**< ID of the vertex array object storing the layout of the mesh. */
    static uint VBO; /**< ID of the vertex buffer object storing the vertices of the mesh. */
    static uint EBO; /**< ID of the element buffer object storing the vertices' draw order. */
};

#endif