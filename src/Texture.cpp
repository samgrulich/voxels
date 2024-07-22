#include "Texture.h"
#include "GLCommon.h"
#include "stb_image/stb_image.h"

#include <iostream>

Texture::Texture(const char* path, int format) {
    GLCall(glGenTextures(1, &ID));
    GLCall(glBindTexture(GL_TEXTURE_2D, ID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    unsigned char* data = stbi_load(path, &width_, &height_, &nrChannels_, 0);
    if (data) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &ID));
}

void Texture::bind(unsigned int slot) {
    GLCall(glActiveTexture(GL_TEXTURE0+slot));
    glBindTexture(GL_TEXTURE_2D, ID);
}
