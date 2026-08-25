#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "BasicShape.hpp"

/** \class Surface
 *  Allows to draw a flat rectangle for surface tessellation.
 */
class Surface : public BasicShape {
public:

    /**
     * Creates a flat rectangle mesh with 4*resolution*resolution triangles, scale according the size.
     * @param size  the scale of the mesh.
     */
    explicit Surface(glm::ivec2 size);
    ~Surface() override;

    /** Binds the VAO of the surface's mesh. */
    void load() const;

    /**
     * Draws the surface.
     * @param shader
     */
    void draw(const Shader & shader) const override;

    static uint resolution; /**< Determines the amount of details (triangles) of the surface.*/

private:

    uint VAO; /**< ID of the vertex array object storing the layout of the mesh. */
    uint VBO; /**< ID of the vertex buffer object storing the vertices of the mesh. */
};


#endif
