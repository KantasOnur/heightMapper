#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Core/Window.h"
#include "Scene.h"

class Game
{
private:

    static std::unique_ptr<Window> window_;
    static std::unique_ptr<Scene> scene_;

    /*
    static std::unique_ptr<Input> input_;
    static std::unique_ptr<Camera> camera_;
    */

public:
    Game();
    void run();

};

#endif //GAME_H
