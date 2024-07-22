#include "Game.h"
#include "Core/Gui.h"

std::unique_ptr<Window> Game::window_;
std::unique_ptr<Scene> Game::scene_;
//std::unique_ptr<EventManager> Game::manager_;
std::unique_ptr<Input> Game::input_;
std::unique_ptr<Camera> Game::camera_;
std::unique_ptr<Gui> Game::gui_;
bool Game::openGui = false;

Game::Game()
{
    window_ = std::make_unique<Window>(640, 480);
    gui_ = std::make_unique<Gui>(window_->getPtr());
    scene_ = std::make_unique<Scene>();
    //manager_ = std::make_unique<EventManager>();
    input_ = std::make_unique<Input>();
    camera_ = std::make_unique<Camera>();
}

void Game::run()
{
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );



    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    while(window_->isOpen())
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera_->update(window_->getDeltaTime());
        scene_->render();
        window_->update();
    }
}