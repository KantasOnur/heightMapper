#ifndef EROSION_H
#define EROSION_H

#include <glm/glm.hpp>
#include <vector>

struct Particle
{
    Particle(const glm::vec2& position) {pos = position;}
    glm::vec2 pos;
    glm::vec2 dir = glm::vec2(0.0f); // Normalized
    float vel = 1.0f; // Speed
    float water = 1.0f;
    float sediment = 0.0f;
};

struct uvCoords
{
    float u;
    float v;
};

struct heightAndGradient
{
    float height;
    glm::vec2 gradient;
};

struct Params
{
    float intertia = 0.3f;
    float minSlope = 0.0001f;
    float capacity = 4.0f;
    float minSediment = 0.1f;
    float deposition = 0.9f; // Percentage of sediment surplus dropped.
    float erosion = 0.1f; // Percenteage of remaining capacity used for erosion.
    float gravity = 4.0f;
    float evaporation = 0.1f;
    int radius = 6.0f;
};
using heightMap = std::vector<unsigned char>;
using perlinMap = std::vector<float>;
using namespace glm;
class Erosion
{
public:
    void static Erode(std::vector<float>& map, const int& mapSize);
};


#endif //EROSION_H
