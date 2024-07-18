#include "Erosion.h"

#include <iostream>
#include <random>
#include <glm/ext/quaternion_common.hpp>

std::default_random_engine generator;

float InverseLerp(float a, float b, float value)
{
    if (a != b)
    {
        return std::clamp((value - a) / (b - a), 0.0f, 1.0f);
    }
        return 0.0f; // or any appropriate value when a == b
}

perlinMap translateToPerlinMap(const heightMap& map, const int& mapSize)
{
    perlinMap perlinValues(mapSize * mapSize);
    for(int y = 0; y < mapSize; ++y)
    {
        for(int x = 0; x < mapSize; ++x)
        {
            perlinValues[y*mapSize + x] = InverseLerp(0, 255, map[y*mapSize + x]);
        }
    }
    return perlinValues;
}

vec2 getRandomPosition(int mapSize)
{
    std::uniform_int_distribution<int> distribution(1,mapSize-2);
    return {distribution(generator), distribution(generator)};
}

bool isOutOfTheMap(const vec2& position, const int& mapSize)
{
    return position.x <= 0 || position.x >= mapSize - 1 || position.y <= 0 || position.y >= mapSize -1;
}

heightAndGradient calculateHeightAndGradient(const perlinMap& map, const vec2& position, const uvCoords& cellOffset, const int& mapSize)
{
    /* Since our cell is a unit square we can simplify the calculation to:
     *
     * bilinear interpolation: f(x,y) = (1−x)*(1−y)*f00 + x*(1−y)*f10 + (1−x)*y*f01 + x*y*f11
     * gradient vector(f(x,y)) = vec2(d/dx, d/dy)
     */

    float x = cellOffset.u;
    float y = cellOffset.v;

    float f00 = map[position.y * mapSize + position.x];
    float f10 = map[position.y * mapSize + (position.x+1)];
    float f01 = map[(position.y+1)*mapSize + position.x];
    float f11 = map[(position.y+1)*mapSize + (position.x+1)];

    vec2 gradient = vec2((f10-f00)*(1-y) + (f11-f01)*y, (f01-f00)*(1-x) + (f11-f10)*x);
    float height = (1-x)*(1-y)*f00 + x*(1-y)*f10 + (1-x)*y*f01 + x*y*f11;
    return {height, gradient};
}

void deposit(const vec2& position, heightMap& map, const int& mapSize, const float& amount)
{
    int ix = (int) position.x;
    int iy = (int) position.y;
    double u = position.x - ix;
    double v = position.y - iy;
    map[iy*mapSize + ix] += amount * (1 - u) * (1 - v);
    map[iy*mapSize + (ix+1)] += amount * u * (1 - v);
    map[(iy+1)*mapSize + ix] += amount * (1 - u) * v;
    map[(iy+1)*mapSize + (ix+1)] += amount * u * v;

}

perlinMap Erosion::Erode(heightMap map, const int& mapSize)
{
    /*
     * Goal: the drop is moved for each iteration of its lifetime from current position, posOld.
     */

    Particle particle(getRandomPosition(mapSize));
    perlinMap perlinValues = translateToPerlinMap(map, mapSize);
    Params params;
    for(int i = 0; i < 30; ++i)
    {
        uvCoords cellOffsetOld = {(int)particle.pos.x - particle.pos.x, (int)particle.pos.y-particle.pos.y};
        vec2 posOld = particle.pos;
        heightAndGradient hg = calculateHeightAndGradient(perlinValues, posOld, cellOffsetOld, mapSize);
        float heightOld = hg.height;

        particle.dir = particle.dir*params.intertia - hg.gradient*(1-params.intertia);
        if(length(particle.dir)== 0)
            break;
        particle.dir = normalize(particle.dir);
        particle.pos += particle.dir;

        if(isOutOfTheMap(particle.pos, mapSize))
            break;

        uvCoords cellOffsetNew = {(int)particle.pos.x - particle.pos.x, (int)particle.pos.y-particle.pos.y};
        float heightNew = calculateHeightAndGradient(perlinValues, posOld, cellOffsetNew, mapSize).height;

        float heightDiff = heightNew - heightOld;
        float capacity = max(-heightDiff, params.minSlope)*particle.vel*particle.water*params.capacity;

        if(particle.sediment > capacity)
        {
            float amount = (particle.sediment-capacity)*params.deposition;
            std::cout << capacity << std::endl;
            particle.sediment -= amount;
            deposit(posOld, map, mapSize, amount);
        }
        else
        {
            //erode();
        }
    }

    return perlinValues;
}
