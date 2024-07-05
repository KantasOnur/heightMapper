#ifndef TEXTURE_H
#define TEXTURE_H
#include <memory>
#include "Shader.h"
class Texture
{
private:
    unsigned int id_;
    unsigned int width_;
    unsigned int height_;
public:
    Texture(const std::vector<unsigned char>& textureData, const int& width, const int& height);
    ~Texture();

    void updateTexture(const std::vector<unsigned char>& textureData);
    void bind();
    void unbind();
};
#endif //TEXTURE_H
