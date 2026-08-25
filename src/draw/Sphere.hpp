#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../utility/Shader.hpp"
#include "BasicShape.hpp"

/** \class Sphere
 * Allows to draw a sphere.
 */
class Sphere final : public BasicShape {
public:

    Sphere() = default;

    /** Binds the vertex array of the sphere. Generates the VAO, VBO and EBO if necessary */
    static void load();

    /** Deletes the VAO, VBO and EBO */
    static void free();

    /**
     * Draws the sphere with its attributes.
     * @param shader
     */
    void draw(const Shader & shader) const override;


    const static int latResolution; /**< Smoothness latitude factor of the sphere's mesh */
    const static int lonResolution; /**< Smoothness longitude factor of the sphere's mesh. */

private:

    static uint VAO; /**< ID of the vertex array object storing the layout of the mesh. */
    static uint VBO; /**< ID of the vertex buffer object storing the vertices of the mesh. */
    static uint EBO; /**< ID of the element buffer object storing the vertices' draw order. */
};

#endif
