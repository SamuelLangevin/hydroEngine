#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP
#include "../resources/ResourceRefs.hpp"


class MeshBuilder {
    public:
        MeshBuilder() = delete;
        static MeshRef getSphereMesh();
        static MeshRef getBoxMesh();
        static MeshRef getSurfaceMesh();

    private:
        static MeshRef createSphereMesh();
        static MeshRef createBoxMesh();
        static MeshRef createSurfaceMesh();

};

#endif