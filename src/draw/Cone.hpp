#ifndef CONE_HPP
#define CONE_HPP
#include "BasicShape.hpp"

/** \class Cone
 * Allows to draw cones.
 */
class Cone final : public BasicShape{
public:

    Cone() = default;

    /** Deletes the VAO, VBO and EBO */
    static void free();

    /** Binds the cone VAO. Generates it if necessary. */
    static void load();

    /**< Draws the cone with its uniforms. */
    void draw(const Shader & shader) const override;

    const static int lonResolution; /**< Smoothness longitude factor of the sphere's mesh. */

private:

    static uint VAO; /**< ID of the vertex array object storing the layout of the mesh. */
    static uint VBO; /**< ID of the vertex buffer object storing the vertices of the mesh. */
    static uint EBO; /**< ID of the element buffer object storing the vertices' draw order. */
};


#endif
