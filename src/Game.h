#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Core/Gui.h"
#include "Scene.h"


class Game
{
private:

    static std::unique_ptr<Window> window_;
    static std::unique_ptr<Scene> scene_;
    static std::unique_ptr<Input> input_;
    static std::unique_ptr<Camera> camera_;
    static std::unique_ptr<Gui> gui_;
public:

    Game();
    void run();

    static Window& getWindow() {return *window_;}
    static Input& getInput() {return *input_;}
    static Camera& getCamera() {return *camera_;}
    static Gui& getGui() {return *gui_;}

    static bool openGui;

};

#endif //GAME_H
