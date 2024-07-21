#define GLM_ENABLE_EXPERIMENTAL
#include "Scene.h"

#include <iostream>
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Core/Gui.h"
#include "Game.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/noise.hpp>
#include "Erosion/Erosion.h"
#include <fastNoise/FastNoise.h>
#include <random>

using namespace glm;
bool flag = true;

std::unique_ptr<Mesh> water;
std::unique_ptr<Mesh> terrain;
std::unique_ptr<Texture> map;

std::unique_ptr<Shader> waterShader;
std::unique_ptr<Shader> terrainShader;
int iteration = 0;

float inverseLerp(float a, float b, float value)
{
    if (a != b)
    {
        return std::clamp((value - a) / (b - a), 0.0f, 1.0f);
    }
    else
    {
        return 0.0f; // or any appropriate value when a == b
    }
}

std::vector<unsigned char> generateNoise(const noiseParams& params)
{

    FastNoise noise;
    noise.SetNoiseType(FastNoise::PerlinFractal);
    noise.SetFractalType(FastNoise::FBM);

    noise.SetFractalOctaves(params.octaves);
    noise.SetFractalGain(params.persistance);
    noise.SetFractalLacunarity(params.lacunarity);
    noise.SetSeed(params.seed);

    float min = 0.0f;
    float max = 0.0f;
    std::vector<unsigned char> noiseMap(256 * 256);
    std::vector<float> perlinValues(256 * 256);
    int index = 0;
    for(int y = 0; y < 256; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            float perlinValue = noise.GetNoise(x/params.scale, y/params.scale);
            if(perlinValue < min)
                min = perlinValue;
            if(perlinValue > max)
                max = perlinValue;
            perlinValues[index] = perlinValue;
            index++;
        }
    }

    index = 0;
    for(int y = 0; y < 256; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            noiseMap[y * 256 + x] = inverseLerp(min, max, perlinValues[index]) * 255;
            index++;
        }
    }

    return noiseMap;
}
std::vector<Vertex> generatePlaneVertices(int div, float width) {
    std::vector<Vertex> vertices;
    float triangleSide = width / div;
    float stepSize = 1.0f / div;
    for (int row = 0; row < div + 1; ++row) {
        for (int col = 0; col < div + 1; ++col) {

            glm::vec3 position(col * triangleSide, 0.0f, -row * triangleSide);
            glm::vec3 color(0.368f, 0.96f, 0.822f);
            glm::vec2 uvCoords(stepSize * col, stepSize * row);
            Vertex vertex = { position, color, uvCoords };
            vertices.push_back(vertex);
        }
    }
    return vertices;
}
std::vector<Index> generatePlaneIndices(int div)
{
    std::vector<Index> indices;
    for(int row = 0; row < div; ++row)
    {
        for(int col = 0; col < div; ++col)
        {
            int index = row * (div + 1) + col;
            indices.push_back(index);
            indices.push_back(index + (div + 1) + 1);
            indices.push_back(index + (div + 1));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + (div + 1) + 1);
        }
    }
    return indices;
}

Scene::Scene()
{
    std::vector<Vertex> terrainVertices = generatePlaneVertices(2000, 4);
    std::vector<Index> terrainIndices = generatePlaneIndices(2000);
    terrain = std::make_unique<Mesh>(terrainVertices, terrainIndices);
    terrainShader = std::make_unique<Shader>("../shaders/terrain.vert", "../shaders/terrain.frag");

    map = std::make_unique<Texture>(256, 256);

    terrainShader->bind();
    terrainShader->setInt("tex0", 0);
    terrainShader->unbind();
}



static void drawWater(Shader& shader)
{
    shader.bind();
    map->bind();
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    shader.setFloat1f("u_time", Game::getWindow().getTime());
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    water->draw(shader);
    map->unbind();

}

void drawTerrain(Shader& shader)
{
    shader.bind();
    map->bind();
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    terrain->draw(shader);
    map->unbind();

}

heightMap formatErodedMap(const perlinMap& erodedMap)
{
    std::vector<unsigned char> noiseMap(256 * 256);

    int index = 0;
    for(int y = 0; y < 256; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            noiseMap[y * 256 + x] = max(0.0f, erodedMap[index]) * 255;
            index++;
        }
    }

    return noiseMap;
}

heightMap blurHeightMap(const heightMap& map, const int& mapSize)
{
    heightMap blurredMap(mapSize * mapSize);

    for (int y = 0; y < mapSize; ++y)
    {
        for (int x = 0; x < mapSize; ++x)
        {
            int targetPixel = y * mapSize + x;

            if (x < 1 || y < 1 || x + 1 == mapSize || y + 1 == mapSize)
            {
                blurredMap[targetPixel] = map[targetPixel];
            }
            else
            {
                int sum = map[(y - 1) * mapSize + (x - 1)] +  // Top left
                          map[(y - 1) * mapSize + x] +         // Top center
                          map[(y - 1) * mapSize + (x + 1)] +  // Top right
                          map[y * mapSize + (x - 1)] +        // Mid left
                          map[y * mapSize + x] +              // Current pixel
                          map[y * mapSize + (x + 1)] +        // Mid right
                          map[(y + 1) * mapSize + (x - 1)] +  // Low left
                          map[(y + 1) * mapSize + x] +        // Low center
                          map[(y + 1) * mapSize + (x + 1)];   // Low right

                blurredMap[targetPixel] = static_cast<unsigned char>(sum / 9);
            }
        }
    }

    return blurredMap;
}
heightMap mixBlur(const heightMap& map, const int& mapSize)
{
    heightMap blurredMap = blurHeightMap(map, mapSize);
    heightMap mixBlurred(mapSize * mapSize);
    for(int y = 0; y < mapSize; ++y)
    {
        for(int x = 0; x < mapSize; ++x)
        {
            mixBlurred[y*mapSize+x] = 0.7*blurredMap[y*mapSize+x]+(1-0.7)*map[y*mapSize+x];
        }
    }
    return mixBlurred;
}
void Scene::render()
{
    Game::getGui().beginFrame();

    auto updated = Game::getGui().noiseParamsUpdated();
    if(updated.isUpdated)
    {
        map->updateTexture(generateNoise(updated.recentParams));
    }

    bool prevState = Game::getGui().isErosionEnabled;
    if(Game::getGui().toggleErode())
    {
        perlinMap erodedMap = Erosion::Erode(map->getMap(), 256);
        map->updateTexture(formatErodedMap(erodedMap));
        iteration++;
        std::cout << iteration << std::endl;

    }
    else if (prevState && !Game::getGui().isErosionEnabled)
    {
        map->updateTexture(mixBlur(map->getMap(), 256));
    }

    drawTerrain(*terrainShader);
    Game::getGui().endFrame();
}

