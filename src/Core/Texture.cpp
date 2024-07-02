#include "Texture.h"

#include <memory>
#include <GL/glew.h>
#include <stb/stb_image.h>
Texture::Texture(const char* filename, const std::unique_ptr<Shader>& shader)
{
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(filename, &width, &height, &nChannels, 0);
    glGenTextures(1, &id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /*why is opengl like this?!! https://stackoverflow.com/questions/59547366/how-do-i-display-a-grayscale-image-with-opengl-texture*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    shader->setInt("tex0", 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id_);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
