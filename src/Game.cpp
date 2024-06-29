#include "Game.h"

std::unique_ptr<Window> Game::window_;
std::unique_ptr<Scene> Game::scene_;

Game::Game()
{
    window_ = std::make_unique<Window>(640, 480);
    scene_ = std::make_unique<Scene>();
}

void Game::run()
{

    while(window_->isOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene_->render();
        window_->update();
    }
}