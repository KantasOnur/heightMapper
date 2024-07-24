#ifndef GUI_H
#define GUI_H

#include <memory>
#include <GLFW/glfw3.h>
//#include "Shader.h"

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
struct uniformParams
{
    float grassThreshold;
};
class Gui
{
private:
    std::unique_ptr<noiseParams> noiseParamsPtr;
    std::unique_ptr<uniformParams> uniformParamsPtr;
    bool isBlurEnabled;
public:
    Gui(GLFWwindow* window);
    ~Gui();
    void beginFrame();
    void endFrame();
    void addText(const int& val);
    const noiseParams getNoiseParams();
    bool toggleErode();
    bool toggleBlur();
    uniformParams getUniformParams();
    boolParams noiseParamsUpdated();
    bool isErosionEnabled;
};
#endif //GUI_H
