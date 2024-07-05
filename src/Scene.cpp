#define GLM_ENABLE_EXPERIMENTAL
#include "Scene.h"

#include <iostream>
#include "Core/Mesh.h"
#include "Core/Texture.h"

#include "Game.h"
#include <glm/gtx/string_cast.hpp>
#include <fastNoise/FastNoise.h>

using namespace glm;

struct Noise
{
    int count;
    float frequency;
    float amplitude;
    int lacunarity;
    int seed;
};

std::unique_ptr<Noise> noiseParams;

std::unique_ptr<Mesh> water;
std::unique_ptr<Mesh> terrain;
std::unique_ptr<Texture> heightMap;

std::unique_ptr<Shader> waterShader;
std::unique_ptr<Shader> terrainShader;


std::vector<unsigned char> generateNoise(const std::unique_ptr<Noise>& noiseParams)
{

    FastNoise noise;
    noise.SetNoiseType(FastNoise::PerlinFractal);
    noise.SetSeed(noiseParams->seed);
    noise.SetFractalOctaves(noiseParams->count);
    noise.SetFractalLacunarity(noiseParams->lacunarity);
    noise.SetFrequency(noiseParams->frequency);
    noise.SetFractalGain(noiseParams->amplitude);

    std::vector<unsigned char> noiseData(512 * 512);
    int index = 0;
    for (int y = 0; y < 512; y++)
    {
        for (int x = 0; x < 512; x++)
        {

            float noiseValue = (noise.GetNoise((float)x, (float)y) + 1) * 0.5;
            float normalizedValue = noiseValue * 255.0f;
            unsigned char noiseByte = static_cast<unsigned char>(normalizedValue);
            noiseData[index++] = noiseByte;
        }
    }
    return noiseData;

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

    noiseParams = std::make_unique<Noise>();
    noiseParams->count = 0;
    noiseParams->frequency = 0.0f;
    noiseParams->amplitude = 0.0f;


    std::vector<Vertex> terrainVertices = generatePlaneVertices(1024, 3);
    std::vector<Index> terrainIndices = generatePlaneIndices(1024);
    terrain = std::make_unique<Mesh>(terrainVertices, terrainIndices);
    terrainShader = std::make_unique<Shader>("../shaders/terrain.vert", "../shaders/terrain.frag");

    std::vector<unsigned char> noise = generateNoise(noiseParams);
    heightMap = std::make_unique<Texture>(noise, 512, 512);
    terrainShader->bind();
    terrainShader->setFloat1f("triangleSide", 3.0f/1024.0f);
    terrainShader->setFloat1f("stepSize", 1.0f/1024.0f);
    terrainShader->setInt("tex0", 0);
    terrainShader->unbind();

    std::vector<Vertex> waterVertices = generatePlaneVertices(1000, 10);
    std::vector<Index> waterIndices = generatePlaneIndices(1000);
    water = std::make_unique<Mesh>(terrainVertices, terrainIndices);
    waterShader = std::make_unique<Shader>("../shaders/water.vert", "../shaders/water.frag");
    waterShader->bind();
    waterShader->setInt("tex0", 0);
    waterShader->unbind();
}



static void drawWater(Shader& shader)
{
    shader.bind();
    heightMap->bind();
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    shader.setFloat1f("u_time", Game::getWindow().getTime());
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    water->draw(shader);
    heightMap->unbind();

}

void drawTerrain(Shader& shader)
{
    shader.bind();
    heightMap->bind();
    shader.setVec3f("lightPos", Game::getCamera().getPosition());
    shader.setVec3f("viewPos", Game::getCamera().getPosition());
    shader.setMatrix4f("u_projectionMatrix", Game::getCamera().getProjection());
    shader.setMatrix4f("u_modelViewMatrix", Game::getCamera().getView());
    terrain->draw(shader);
    heightMap->unbind();

}
void Scene::render()
{
    if (Game::openGui)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& gui = Game::getGui();
        ImGui::Begin("Noise Parameters");

        ImGui::SliderInt("Noise count", &noiseParams->count, 0, 20);
        ImGui::SliderFloat("Noise frequency", &noiseParams->frequency, 0.0f, 0.5f);
        ImGui::SliderFloat("Noise amplitude", &noiseParams->amplitude, 0.0f, 1.0f);
        ImGui::SliderInt("Noise lacunarity", &noiseParams->lacunarity,0 ,10);
        ImGui::SliderInt("Noise Seed", &noiseParams->seed, 0, 3000);
        heightMap->updateTexture(generateNoise(noiseParams));
    }



    //drawWater(*waterShader);
    drawTerrain(*terrainShader);

    if(Game::openGui)
    {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

