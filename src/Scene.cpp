#define GLM_ENABLE_EXPERIMENTAL
#include "Scene.h"

#include <iostream>
#include <fstream>
#include "Core/Mesh.h"
#include "Core/Texture.h"

#include "Game.h"
#include <glm/gtx/string_cast.hpp>
#include <fastNoise/FastNoise.h>

using namespace glm;

std::unique_ptr<Mesh> water;
std::unique_ptr<Mesh> terrain;
//std::unique_ptr<Texture> heightMap;

std::unique_ptr<Shader> waterShader;
std::unique_ptr<Shader> terrainShader;
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

    std::vector<Vertex> terrainVertices = generatePlaneVertices(1024, 10);
    std::vector<Index> terrainIndices = generatePlaneIndices(1024);
    terrain = std::make_unique<Mesh>(terrainVertices, terrainIndices);
    terrainShader = std::make_unique<Shader>("../shaders/terrain.vert", "../shaders/terrain.frag");

    FastNoise noise;
    noise.SetNoiseType(FastNoise::PerlinFractal);
    noise.SetFrequency(0.003);
    noise.SetFractalOctaves(10);

    std::vector<float> noiseData(256 * 256);
    std::vector<unsigned char> normalizedData(256 * 256);
    int index = 0;
    for (int y = 0; y < 256; y++)
    {
        for (int x = 0; x < 256; x++)
        {

            //std::cout << pow((noise.GetNoise((float)x, (float)y) + 1) * 0.5, 1.4) << std::endl;
            float noiseValue = noise.GetNoise((float)x, (float)y);
            float normalizedValue = std::clamp(noiseValue, 0.0f, 1.0f) * 255.0f;
            unsigned char noiseByte = static_cast<unsigned char>(normalizedValue);
            normalizedData[index++] = noiseByte;
        }
    }

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 256, 0, GL_RED, GL_UNSIGNED_BYTE, normalizedData.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    terrainShader->bind();
    terrainShader->setInt("tex0", 0);
    terrainShader->unbind();


  /*
    terrain = std::make_unique<Mesh>(terrainVertices, terrainIndices);
    terrainShader = std::make_unique<Shader>("../shaders/terrain.vert", "../shaders/terrain.frag");
    terrainShader->bind();
    terrainShader->setFloat1f("triangleSide",(float) 10 / (float) 1000);
    terrainShader->setFloat1f("stepSize", 1.0f/(float) 1000);
    terrainShader->unbind();
    heightMap = std::make_unique<Texture>("../assets/heightMap.png", terrainShader);

*/


    int div = 400;
    int width = 10;
    std::vector<Vertex> planeVertices = generatePlaneVertices(div, width);
    std::vector<Index> planeIndices = generatePlaneIndices(div);

    water = std::make_unique<Mesh>(planeVertices, planeIndices);
    waterShader = std::make_unique<Shader>("../shaders/water.vert", "../shaders/water.frag");

/*
    int div2 = 10;
    int width2 = 10;
    terrain = std::make_unique<Mesh>(generatePlaneVertices(3, 10), generatePlaneIndices(3));
    terrainShader = std::make_unique<Shader>("../shaders/terrain.vert", "../shaders/terrain.frag");
    terrainShader->bind();
    terrainShader->setFloat1f("triangleSide",(float) width2 / (float) div2);
    terrainShader->setFloat1f("stepSize", 1.0f/(float) div2);
    terrainShader->unbind();
    heightMap = std::make_unique<Texture>("../assets/heightMap.png", terrainShader);
    */
}



static void drawWater(Shader& shader)
{
    shader.bind();
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    shader.setFloat1f("u_time", Game::getWindow().getTime());
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    water->draw(shader);
}

/*
static void drawTerrain(Shader& shader, Texture& texture)//, Texture& texture)
{

    shader.bind();
    texture.bind();
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    terrain->draw(shader);
    texture.unbind();
}
*/
void drawTerrain(Shader& shader, unsigned int texture)
{
    shader.bind();
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());

    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    terrain->draw(shader);

}
void Scene::render()
{
    drawWater(*waterShader);
    drawTerrain(*terrainShader, texture);//, *heightMap);//, *heightMap);
}

