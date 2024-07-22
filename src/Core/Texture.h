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
    std::vector<float> map;
public:
    Texture(const std::vector<float>& textureData, const int& width, const int& height);
    ~Texture();

    void updateTexture(const std::vector<float>& textureData);
    void bind();
    void unbind();

    std::vector<float>& getMap() {return map;}
};
#endif //TEXTURE_H
