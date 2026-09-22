#ifndef RESOURCE_REFS_HPP
#define RESOURCE_REFS_HPP
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "../../includes/glad.h"

struct MeshRef {
    private:
        uint vaoID;
        uint vertexDrawCount;
        GLenum drawMode;
        bool useIndices;

    public:
        explicit MeshRef(const uint vaoID, const uint drawCount)
            : vaoID(vaoID), vertexDrawCount(drawCount) {};

        void draw() const {
            glBindVertexArray(vaoID);
            if (useIndices) glDrawElements(drawMode, vertexDrawCount, GL_UNSIGNED_INT, nullptr);
            else glDrawArrays(drawMode, 0, vertexDrawCount);
            glBindVertexArray(0);
     }
};

struct TextureRef {
    private:
        uint textureID;
        GLenum type;

    public:
        explicit TextureRef(const uint texture_id, const GLenum type)
            : textureID(texture_id), type(type) {
        }

        void bind(const Shader & shader, const std::string &name, const int channel) const{
            shader.setInt(name, channel);
            glActiveTexture(GL_TEXTURE0 + channel);
            glBindTexture(type, this->textureID);
        }
};

#endif