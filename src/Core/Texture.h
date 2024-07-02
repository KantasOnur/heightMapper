#ifndef TEXTURE_H
#define TEXTURE_H
#include <memory>
#include "Shader.h"
class Texture
{
private:
    unsigned int id_;
public:
    Texture(const char* filename, const std::unique_ptr<Shader>& shader);
    ~Texture();

    void bind();
    void unbind();
};
#endif //TEXTURE_H
