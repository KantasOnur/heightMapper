#include "Scene.h"
#include "Core/Mesh.h"
#include <glm/glm.hpp>

using namespace glm;

std::unique_ptr<Mesh> triangle;

std::unique_ptr<Shader> sceneShader;

Scene::Scene()
{
    std::vector<Vertex> vertices =
    {
        {vec3(-0.5f, -0.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f)},
        {vec3(0.5f, -0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)},
        {vec3(0.0f,  0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f)}
    };
    std::vector<Index> indices =
    {
        0, 1, 2
      };

    triangle = std::make_unique<Mesh>(vertices, indices);
    sceneShader = std::make_unique<Shader>("../shaders/scene.vert", "../shaders/scene.frag");
}

static void drawTriangle(Shader& shader)
{
    shader.bind();
    triangle->draw(shader);
}
void Scene::render()
{
    drawTriangle(*sceneShader);
}

