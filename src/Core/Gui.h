#ifndef GUI_H
#define GUI_H
#include <memory>
#include <ImGui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

struct noiseParams
{
    float scale;
    int octaves;
    float persistance;
    float lacunarity;
    int seed;
    float offsetX;
    float offsetY;
};
struct boolParams
{
    bool isUpdated;
    noiseParams recentParams;
};
class Gui
{
private:
    std::unique_ptr<noiseParams> noiseParamsPtr;
    bool isErosionEnabled;
public:
    Gui(GLFWwindow* window);
    ~Gui();
    void beginFrame();
    void endFrame();

    const noiseParams getNoiseParams();
    bool toggleErode();
    boolParams noiseParamsUpdated();
};
#endif //GUI_H
