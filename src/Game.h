#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Events/EventManager.h"
#include "Scene.h"

class Game
{
private:

    static std::unique_ptr<Window> window_;
    static std::unique_ptr<Scene> scene_;
    //static std::unique_ptr<EventManager> manager_;
    static std::unique_ptr<Input> input_;
    static std::unique_ptr<Camera> camera_;

public:
    Game();
    void run();

    static Window& getWindow() {return *window_;}
    //static EventManager& getManager() {return *manager_;}
    static Input& getInput() {return *input_;}
    static Camera& getCamera() {return *camera_;}
};

#endif //GAME_H
