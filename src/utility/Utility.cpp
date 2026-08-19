#include "Utility.hpp"
#include <iostream>
#include <map>
#include "Texture.hpp"

void Utility::createMultiSampleFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize) {
    glGenFramebuffers(1, &FBO.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);

    auto generateTextureBuffer = [&](uint & textureBufferId, const GLuint number) {
        glGenTextures(1, &textureBufferId);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBufferId);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, windowSize.x, windowSize.y, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, number, GL_TEXTURE_2D_MULTISAMPLE, textureBufferId, 0);
    };
    generateTextureBuffer(FBO.textureBuffer0, GL_COLOR_ATTACHMENT0);
    generateTextureBuffer(FBO.textureBuffer1, GL_COLOR_ATTACHMENT1);

    glGenRenderbuffers(1, &FBO.renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, FBO.renderBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FBO.renderBuffer);
    uint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Multisample framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Utility::createFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize, bool attachRenderBuffer, int colorAttachments) {
    glGenFramebuffers(1, &FBO.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);

    FBO.textureBuffer0 = Texture::createTexture(windowSize, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_BORDER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO.textureBuffer0, 0);

    if (colorAttachments == 2 ) {
        FBO.textureBuffer1 = Texture::createTexture(windowSize, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_BORDER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBO.textureBuffer1, 0);
    }

    if (attachRenderBuffer) {
        glGenRenderbuffers(1, &FBO.renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, FBO.renderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FBO.renderBuffer);
    }
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Utility::createGBuffer(GeometryFrameBuffer & FBO, glm::ivec2 windowSize) {
    glGenFramebuffers(1, &FBO.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);

    FBO.positionTexture = Texture::createTexture(windowSize, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_BORDER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO.positionTexture, 0);
    FBO.normalTexture = Texture::createTexture(windowSize, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_BORDER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBO.normalTexture, 0);
    FBO.albedoSpecTex = Texture::createTexture(windowSize, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_BORDER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, FBO.albedoSpecTex, 0);

    uint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &FBO.renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, FBO.renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FBO.renderBuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Utility::createDepthFrameBuffer(FrameBuffer & FBO, int shadowsResolution) {
    glGenFramebuffers(1, &FBO.ID);
    FBO.textureBuffer0 = Texture::createTexture(glm::ivec2(shadowsResolution),
        GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT,GL_FLOAT,GL_CLAMP_TO_BORDER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, FBO.textureBuffer0);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBO.textureBuffer0, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Utility::createDepthCubeMapFrameBuffer(FrameBuffer & FBO, int shadowsResolution) {
    glGenFramebuffers(1, &FBO.ID);
    glGenTextures(1, &FBO.textureBuffer0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, FBO.textureBuffer0);
    for (int i = 0; i < 6; ++i) {
       glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowsResolution,
            shadowsResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, FBO.textureBuffer0, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Utility::createSSAOFRameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize) {
    glGenFramebuffers(1, &FBO.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);

    glGenTextures(1, &FBO.textureBuffer0);
    glBindTexture(GL_TEXTURE_2D, FBO.textureBuffer0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowSize.x, windowSize.y,
                0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO.textureBuffer0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Utility::createHDRCubemapFramebuffer(FrameBuffer & FBO, glm::ivec2 resolution) {
    glGenFramebuffers(1, &FBO.ID);
    glGenRenderbuffers(1, &FBO.renderBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);
    glBindRenderbuffer(GL_RENDERBUFFER, FBO.renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBO.renderBuffer);
}

GLenum Utility::glCheckError_(const char *file, int line) {

    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default: error = "UNKNOWN_ERROR"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << "\n";
    }
    return errorCode;
}

void Utility::loadFont(FT_Library & ft, Font & characters, const char * fontPath) {

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
        std::cout << "ERROR::FREETYPE: Could not load font\n";
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Could not load Glyph\n";
            continue;
        }

        uint texture = Texture::createTexture(glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
            GL_RED, GL_RED, GL_UNSIGNED_BYTE,GL_CLAMP_TO_EDGE,GL_LINEAR, face->glyph->bitmap.buffer);
        glBindTexture(GL_TEXTURE_2D,texture);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<uint>(face->glyph->advance.x)
        };

        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
}

