#include "Erosion.h"

#include <iostream>
#include <random>
#include <glm/ext/quaternion_common.hpp>


std::default_random_engine generator;

vec2 getRandomPosition(int mapSize)
{
    std::uniform_real_distribution<float> distribution(1,mapSize-1);
    return {distribution(generator), distribution(generator)};
}
bool isOutOfTheMap(const vec2& position, const int& mapSize)
{
    return position.x <= 0 || position.x >= mapSize - 1 || position.y <= 0 || position.y >= mapSize -1;
}
heightAndGradient calculateHeightAndGradient(const std::vector<float>& map, const vec2& position, const int& mapSize)
{
    /* Since our cell is a unit square we can simplify the calculation to:
     *
     * bilinear interpolation: f(x,y) = (1−x)*(1−y)*f00 + x*(1−y)*f10 + (1−x)*y*f01 + x*y*f11
     * gradient vector(f(x,y)) = vec2(d/dx, d/dy)
     */

    int ix = (int)position.x;
    int iy = (int)position.y;


    float x = position.x-(int)position.x;
    float y = position.y-(int)position.y;

    float f00 = map[iy * mapSize + ix];
    float f10 = map[iy * mapSize + (ix+1)];
    float f01 = map[(iy+1)*mapSize + ix];
    float f11 = map[(iy+1)*mapSize + (ix+1)];

    vec2 gradient = vec2((f10-f00)*(1-y) + (f11-f01)*y, (f01-f00)*(1-x) + (f11-f10)*x);
    float height = (1-x)*(1-y)*f00 + x*(1-y)*f10 + (1-x)*y*f01 + x*y*f11;
    return {height, gradient};
}
void deposit(const ivec2& ipos, const vec2& pos, std::vector<float>& map, const int& mapSize, const float& amount)
{
    float u = pos.x - ipos.x;
    float v = pos.y - ipos.y;
    map[ipos.y*mapSize+ipos.x] += amount * (1 - u) * (1 - v);
    map[ipos.y*mapSize + ipos.x+ 1] += amount * u * (1 - v);
    map[(ipos.y+1)*mapSize + ipos.x] += amount * (1 - u) * v;
    map[(ipos.y+1)*mapSize + ipos.x+1] += amount * u * v;
}
void erode(const ivec2& ipos, const vec2& pos, std::vector<float>& map, const int& mapSize, const float& amount, const int& radius)
{
    int xStart = max(ipos.x-radius, 1);
    int yStart = max(ipos.y-radius, 1);

    int xEnd = min(mapSize-2, xStart*2*radius+1);
    int yEnd = min(mapSize-2, yStart*2*radius+1);

    float weightSum = 0.0f;
    float weights[xEnd-xStart][yEnd-yStart];
    for(int y = yStart; y < yEnd; ++y)
    {
        for(int x = xStart; x < xEnd; ++x)
        {
            float weight = max(0.0f, radius - length(vec2(x,y)-pos));

            weightSum += weight;
            weights[x-xStart][y-yStart] = weight;
        }
    }
    for(int y = yStart; y < yEnd; ++y)
    {
        for(int x = xStart; x < xEnd; x++)
        {
            weights[x-xStart][y-yStart] /= weightSum;
            map[y*mapSize + x] -= amount * weights[x-xStart][y-yStart];
        }
    }
}
void Erosion::Erode(std::vector<float>& map, const int& mapSize)
{
    Particle particle(getRandomPosition(mapSize));
    Params params;

    for(int i = 0; i < 30; ++i)
    {
        vec2 posOld = particle.pos;
        heightAndGradient hg = calculateHeightAndGradient(map, posOld, mapSize);
        float heightOld = hg.height;

        particle.dir = particle.dir*params.intertia - hg.gradient*(1-params.intertia);
        if(length(particle.dir) == 0)
            break;
        particle.dir = normalize(particle.dir);
        particle.pos += particle.dir;
        if(isOutOfTheMap(particle.pos, mapSize))
            break;

        float heightNew = calculateHeightAndGradient(map, particle.pos, mapSize).height;
        float heightDiff = heightNew - heightOld;
        float capacity = fmax(-heightDiff, params.minSlope)*particle.vel*particle.water*params.capacity;

        if(particle.sediment > capacity || heightDiff > 0)
        {
            // if the particle going up the terrain then fill up the currrent height.
            float amount = (heightDiff > 0) ? fmin (heightDiff, particle.sediment) : (particle.sediment - capacity) * params.deposition;
            particle.sediment -= amount;
            deposit({posOld.x,posOld.y}, posOld, map, mapSize, amount);
        }
        else
        {
            float amount = fmin((capacity-particle.sediment)*params.erosion, -heightDiff);
            particle.sediment += amount;
            erode({posOld.x,posOld.y}, posOld, map, mapSize, amount, params.radius);
        }

        particle.vel = sqrt(particle.vel*particle.vel + heightDiff*params.gravity);
        particle.water = particle.water*(1-params.evaporation);
    }

}
