#include "Erosion.h"

#include <iostream>
#include <random>
#include <glm/ext/quaternion_common.hpp>

const int sedimentCapacityFactor = 4;

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
    std::uniform_real_distribution<float> distribution(1,mapSize-1);
    return {distribution(generator), distribution(generator)};
}

bool isOutOfTheMap(const vec2& position, const int& mapSize)
{
    return position.x <= 0 || position.x >= mapSize - 1 || position.y <= 0 || position.y >= mapSize -1;
}

heightAndGradient calculateHeightAndGradient(const perlinMap& map, const vec2& position, const int& mapSize)
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


perlinMap Erosion::Erode(heightMap map, const int& mapSize)
{
    /*
     * Goal: the drop is moved for each iteration of its lifetime from current position, posOld.
     */
    perlinMap perlinValues = translateToPerlinMap(map, mapSize);
    Particle particle(getRandomPosition(mapSize));
    Params params;

    for(int i = 0; i < 30; ++i)
    {
        vec2 posOld = particle.pos;
        heightAndGradient hg = calculateHeightAndGradient(perlinValues, posOld, mapSize);
        float heightOld = hg.height;

        particle.dir = particle.dir*params.intertia - hg.gradient*(1-params.intertia);
        if(length(particle.dir)== 0)
            break;
        particle.dir = normalize(particle.dir);
        particle.pos += particle.dir;

        if(isOutOfTheMap(particle.pos, mapSize))
            break;

        float heightNew = calculateHeightAndGradient(perlinValues, particle.pos, mapSize).height;
        float heightDiff = heightNew - heightOld;
        float capacity = fmax (-heightDiff, params.minSlope)*particle.vel*particle.water*params.capacity;
        if(particle.sediment > capacity)
        {
            float amount = (particle.sediment - capacity) * params.deposition;
            particle.sediment -= amount;
            float u = posOld.x - (int)posOld.x;
            float v = posOld.y - (int)posOld.y;
            perlinValues[(int)posOld.y*mapSize+(int)posOld.x] += amount * (1 - u) * (1 - v);
            perlinValues[(int)posOld.y*mapSize + (int)posOld.x+ 1] += amount * u * (1 - v);
            perlinValues[(int)(posOld.y+1)*mapSize + (int)posOld.x] += amount * (1 - u) * v;
            perlinValues[(int)(posOld.y+1)*mapSize + (int)posOld.x+1] += amount * u * v;
        }
        else
        {
            float amount = fmin((capacity-particle.sediment)*params.erosion, -heightDiff);


            int x_start = max((int) (posOld.x-params.radius), 1);
            int y_start = max((int) (posOld.y-params.radius), 1);

            int x_end = min(mapSize-2, x_start*2*params.radius+1);
            int y_end = min(mapSize-2, y_start*2*params.radius+1);

            float weightSum = 0.0f;
            float weights[x_end-x_start][y_end-y_start];
            for(int y = y_start; y < y_end; ++y)
            {
                for(int x = x_start; x < x_end; ++x)
                {
                    float weight = max(0.0f, params.radius - length(vec2(x,y)-posOld));
                    weightSum += weight;
                    weights[x-x_start][y-y_start] = weight;
                }
            }
            for(int y = y_start; y < y_end; ++y)
            {
                for(int x = x_start; x < x_end; x++)
                {
                    weights[x-x_start][y-y_start] /= weightSum;
                    perlinValues[y*mapSize + x] -= amount * weights[x-x_start][y-y_start];
                }
            }
            particle.sediment += amount;

        }

        particle.vel = sqrt(particle.vel*particle.vel + heightDiff*params.gravity);
        particle.water = particle.water*(1-params.evaporation);
    }

    return perlinValues;
}
