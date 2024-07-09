#ifndef EROSION_H
#define EROSION_H

#include <glm/glm.hpp>
#include <vector>

struct Particle
{
    glm::vec2 position;
    glm::vec2 direction;
    float velocity;
    float waterAmount;
    float sedimentAmount;
};

using heightMap = std::vector<unsigned char>;
using namespace glm;
class Erosion
{
public:
    void static Erode(const heightMap& map, const int& mapSize);
};


#endif //EROSION_H
