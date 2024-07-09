#include "Erosion.h"

#include <iostream>
#include <random>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_geometric.hpp>

const float kInertia = .04f;
const float kMinSlope = 0.01f;
const float kSedimentCapacity = 2.0f;
const float kDeposition = 0.1f;
const float kErosion = 0.9f;
const float kGravity = -4;
const float kEvaporation = 0.05f;

std::default_random_engine generator;


float InverseLerp(float a, float b, float value)
{
    if (a != b)
    {
        return std::clamp((value - a) / (b - a), 0.0f, 1.0f);
    }
        return 0.0f; // or any appropriate value when a == b
}

vec2 getRandomPosition(int mapSize)
{
    std::uniform_int_distribution<int> distribution(0,mapSize-1);
    return {distribution(generator), distribution(generator)};
}

float getHeight(const heightMap& map, const int& mapSize, const vec2& position)
{
    return InverseLerp(0.0f, 255.0f, map[position.y * mapSize + position.x]);
}
vec2 calculateGradient(const vec2& dropletPosition, const heightMap& map, const int& mapSize, const vec2& uvCoords)
{
    unsigned char p0 = map[dropletPosition.y * mapSize + dropletPosition.x]; // Px,y
    unsigned char p1 = map[(dropletPosition.y * mapSize) + dropletPosition.x + 1]; // Px+1,y
    unsigned char p2 = map[(dropletPosition.y + 1) * mapSize + dropletPosition.x]; // Px,y+1
    unsigned char p3 = map[(dropletPosition.y + 1) * mapSize + (dropletPosition.x + 1)]; //Px+1,y+1

    float gradientX = (p1 - p0)*(1-uvCoords.y)+(p3 - p2)*uvCoords.y;
    float gradientY = (p2 - p0)*(1-uvCoords.x)+(p3 - p1)*uvCoords.x;

    return {gradientX, gradientY};
}

bool isOutOfTheMap(const vec2& position, const int& mapSize)
{
    return position.x < 0 || position.x > mapSize - 1 || position.y < 0 || position.y > mapSize -1;
}
void Erosion::Erode(const heightMap& map, const int& mapSize)
{
    // Generate a random droplet
    Particle droplet = {getRandomPosition(mapSize), {0,0}, 1.0f, 1.0f, 0.0f};
    // Droplet's uv coordinates on the height map

    float u = InverseLerp(0, mapSize-1, droplet.position.x);
    float v = InverseLerp(0, mapSize-1, droplet.position.y);

    int dropletHeightMapIndex = droplet.position.y * mapSize + droplet.position.x;

    for(int liftetime = 0; liftetime < 30 && !isOutOfTheMap(droplet.position, mapSize); liftetime++)
    {
        vec2 gradientVector = calculateGradient(droplet.position, map, mapSize, {u, v});
        droplet.direction = droplet.direction * kInertia - gradientVector * (1 - kInertia);
        if(length(droplet.direction) == 0)
            break; // Flat ground no need to erode.

        float heightOld = getHeight(map, mapSize, droplet.position);
        droplet.direction = normalize(droplet.direction);
        droplet.position += droplet.direction;
        float heightNew = getHeight(map, mapSize, droplet.position);

        float heightDiff = heightNew - heightOld;

        float carriedSediment = max(-heightDiff, kMinSlope) * droplet.velocity * droplet.waterAmount * kSedimentCapacity;
        if(carriedSediment >= kSedimentCapacity || heightDiff > 0)
        {
            float depositAmount = (droplet.sedimentAmount - carriedSediment) * kDeposition;
            droplet.sedimentAmount -= depositAmount;
            //std::cout << "deposit amount " << depositAmount << std::endl;
        }
        else
        {
            // erode the terrain
            float erosionAmount = min((carriedSediment - droplet.sedimentAmount)*kErosion, -heightDiff);
            //std::cout << "erosion amount "<< carriedSediment << std::endl;
        }

        droplet.velocity = sqrt(abs((droplet.velocity * droplet.velocity)+(heightDiff*kGravity)));
        std::cout << droplet.velocity << std::endl;
        droplet.waterAmount *= (1-kEvaporation);

    }


}
