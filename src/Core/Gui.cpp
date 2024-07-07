#include "Gui.h"
#include <imGui/imgui_impl_glfw.h>
#include "../Game.h"
Gui::Gui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& gui_ = ImGui::GetIO(); (void) gui_;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    noiseParamsPtr = std::make_unique<noiseParams>();
    noiseParamsPtr->octaves = 1;
    noiseParamsPtr->lacunarity = 1.0f;
    noiseParamsPtr->persistance = 0.0f;
    noiseParamsPtr->scale = 1.0f;
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::beginFrame()
{
    if(Game::openGui)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}

const noiseParams Gui::getNoiseParams()
{

    if(Game::openGui)
    {
        ImGui::Begin("Noise Parameters");
        ImGui::SliderFloat("Noise Scale", &noiseParamsPtr->scale, 0.1, 30);
        ImGui::SliderInt("Octaves", &noiseParamsPtr->octaves, 1, 10);
        ImGui::SliderFloat("Lacunarity", &noiseParamsPtr->lacunarity, 1.0f, 5.0f);
        ImGui::SliderFloat("Persistance", &noiseParamsPtr->persistance, 0.0f, 1.0f);
        ImGui::SliderInt("Noise Seed", &noiseParamsPtr->seed, 0, 3000);
        ImGui::SliderFloat("Noise Offset X", &noiseParamsPtr->offsetX, -10, 10);
        ImGui::SliderFloat("Noise Offset Y", &noiseParamsPtr->offsetY, -10, 10);
    }
    return *noiseParamsPtr;

}

void Gui::endFrame()
{
    if(Game::openGui)
    {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

boolParams Gui::noiseParamsUpdated()
{
    noiseParams initialParams = *noiseParamsPtr;
    noiseParams updatedParams = getNoiseParams();

    if(initialParams.lacunarity != updatedParams.lacunarity ||
        initialParams.octaves != updatedParams.octaves ||
        initialParams.persistance != updatedParams.persistance ||
        initialParams.scale != updatedParams.scale ||
        initialParams.seed != updatedParams.seed ||
        initialParams.offsetX != updatedParams.offsetX ||
        initialParams.offsetY != updatedParams.offsetY)
    {
        return {true, updatedParams};
    }
    return {false, updatedParams};
}