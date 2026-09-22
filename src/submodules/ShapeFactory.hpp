#ifndef SHAPE_FACTORY_HPP
#define SHAPE_FACTORY_HPP
#include <stdexcept>

#include "MeshBuilder.hpp"
#include "../scene/Shape.hpp"

class ShapeFactory {
public:
    static Shape createBasicShape(const Shape::ShapeType type, const TextureRef texture) {
        using ST = Shape::ShapeType;
        switch (type) {
            case ST::BOX : {
                const MeshRef mesh = MeshBuilder::getBoxMesh();
                return Shape(mesh, texture, type);
            }
            case ST::SPHERE: {
                const MeshRef mesh = MeshBuilder::getSphereMesh();
                return Shape(mesh, texture, type);
            }
            case ST::SURFACE: {
                const MeshRef mesh = MeshBuilder::getSurfaceMesh();
                return Shape(mesh, texture, type);
            }

            case ST::CUSTOM:
            default: { throw std::invalid_argument("unsupported type"); }
        }
    }

    ShapeFactory() = delete;
};


#endif