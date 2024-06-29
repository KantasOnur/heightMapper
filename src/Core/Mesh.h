#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 colour;
};

using Index = unsigned int;

class Mesh
{
private:
    std::vector<Vertex> vertices_;
    std::vector<Index> indices_;

    unsigned int vao_, vbo_, ibo_;
    void initBuffers();

public:
    Mesh(const std::vector<Vertex>& vertices_, const std::vector<Index>& indices_);
    void draw(Shader& shader);
};

#endif //MESH_H
