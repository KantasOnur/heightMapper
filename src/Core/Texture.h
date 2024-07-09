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
    std::vector<unsigned char> map;
public:
    Texture(const std::vector<unsigned char>& textureData, const int& width, const int& height);
    Texture(const int& width, const int& height);
    ~Texture();

    void updateTexture(const std::vector<unsigned char>& textureData);
    void bind();
    void unbind();

    std::vector<unsigned char> getMap() {return map;}
};
#endif //TEXTURE_H
