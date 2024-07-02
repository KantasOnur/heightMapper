#define GLM_ENABLE_EXPERIMENTAL
#include "Scene.h"

#include <iostream>
#include <fstream>
#include "Core/Mesh.h"
#include "Core/Texture.h"

#include "Game.h"
#include <glm/gtx/string_cast.hpp>

using namespace glm;

std::unique_ptr<Mesh> water;
std::unique_ptr<Mesh> terrain;
std::unique_ptr<Texture> heightMap;

std::unique_ptr<Shader> waterShader;
std::unique_ptr<Shader> terrainShader;

unsigned int texture;


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

    std::vector<Vertex> planeVertices = generatePlaneVertices(400, 10);
    std::vector<Index> planeIndices = generatePlaneIndices(400);

    water = std::make_unique<Mesh>(planeVertices, planeIndices);
    waterShader = std::make_unique<Shader>("../shaders/water.vert", "../shaders/water.frag");


    terrain = std::make_unique<Mesh>(planeVertices, planeIndices);
    terrainShader = std::make_unique<Shader>("../shaders/terrain.vert", "../shaders/terrain.frag");
    terrainShader->bind();
    terrainShader->setFloat1f("triangleSide",10.0f / 1000.0f);
    terrainShader->setFloat1f("stepSize", 1.0f/1000.0f);
    terrainShader->unbind();
    heightMap = std::make_unique<Texture>("../assets/heightMap.png", terrainShader);
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

static void drawTerrain(Shader& shader, Texture& texture)//, Texture& texture)
{

    shader.bind();
    texture.bind();
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    water->draw(shader);
    texture.unbind();
}
void Scene::render()
{
    drawWater(*waterShader);
    drawTerrain(*terrainShader, *heightMap);//, *heightMap);
}

