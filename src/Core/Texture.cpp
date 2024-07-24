#include "Texture.h"

#include <memory>
#include <GL/glew.h>
#include <stb/stb_image.h>
Texture::Texture(const std::vector<float>& textureData, const int& width, const int& height)
    :width_(width), height_(height)
{
    glGenTextures(1, &id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, textureData.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    map = textureData;
}

Texture::~Texture()
{
    glDeleteTextures(1, &id_);
}

void Texture::updateTexture(const std::vector<float>& textureData)
{
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RED, GL_FLOAT, textureData.data());
    unbind();
    map = textureData;
}

void Texture::updateTexture()
{
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RED, GL_FLOAT, map.data());
    unbind();

}
void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
