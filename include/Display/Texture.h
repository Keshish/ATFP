#pragma once

#include <GL/glew.h>
#include "stb/stb_image.h"

#include <iostream>
#include <string>

class Texture {
private:
    GLuint TBO{};

public:
    explicit Texture(const std::string& path) {
        glGenTextures(1, &TBO);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLint format = 0;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, TBO);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         format,
                         width,
                         height,
                         0,
                         static_cast<GLenum>(format),
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cerr << "Texture failed to load at path: " << path << "\n";
            stbi_image_free(data);
        }
    }

    void bind() const {
        glBindTexture(GL_TEXTURE_2D, TBO);
    }

    ~Texture() {
        glDeleteTextures(1, &TBO);
    }
};

