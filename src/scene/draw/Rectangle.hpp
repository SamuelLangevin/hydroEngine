#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "BasicShape.hpp"

/** \class Rectangle
 * Allows to draw rectangle meshes.
 */
class Rectangle : public BasicShape {
public:
    /** Draws a rectangle filling the screen. Used for post-processing. */
    static void draw2DQuad();

    /** Deletes the VAO, VBO and EBO. */
    static void free();
private:

    static uint screenQuadVAO; /**< Vertex array of the screen rectangle.*/
    static uint screenQuadVBO; /**< Vertex buffer of the screen rectangle.*/
};


#endif
