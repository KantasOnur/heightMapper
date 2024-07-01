#define GLM_ENABLE_EXPERIMENTAL
#include "Scene.h"

#include <iostream>

#include "Core/Mesh.h"
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

std::unique_ptr<Mesh> triangle;

std::unique_ptr<Shader> sceneShader;

std::vector<Vertex> generatePlaneVertices(int div, float width)
{
    std::vector<Vertex> vertices;
    float triangleSide = width / div;
    for(int row = 0; row < div + 1; ++row)
    {
        for(int col = 0; col < div + 1; ++col)
        {
            Vertex vertex = {{col * triangleSide, 0.0, row * -triangleSide}, {0.149, 0.4f, 0.5686f}};
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
    std::vector<Vertex> planeVertices = generatePlaneVertices(200, 5);
    std::vector<Index> planeIndices = generatePlaneIndices(200);

    std::cout << planeVertices.size() << std::endl;
    triangle = std::make_unique<Mesh>(planeVertices, planeIndices);
    sceneShader = std::make_unique<Shader>("../shaders/scene.vert", "../shaders/scene.frag");
}



static void drawTriangle(Shader& shader)
{
    shader.bind();
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    shader.setFloat1f("u_time", Game::getWindow().getTime());
    shader.setVec3f("lightPos", {2.5, 1, -2.5});
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    triangle->draw(shader);
}
void Scene::render()
{
    std::cout << "pos: " << glm::to_string(Game::getCamera().getPosition()) << std::endl;
    drawTriangle(*sceneShader);
}

