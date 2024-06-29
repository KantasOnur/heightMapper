#include <GL/glew.h>
#include "Window.h"
#include <iostream>

Window::Window(const int width, const int height)
: width_(width), height_(height)
{
    if(!glfwInit())
    {
        std::cout << "Failed to initialize GLFW." << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    id_ = glfwCreateWindow(width_, height_, "Game", nullptr, nullptr);
    glfwMakeContextCurrent(id_);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW." << std::endl;
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);

}

Window::~Window()
{
    glfwDestroyWindow(id_);
    glfwTerminate();
}

void Window::update() const
{
    glfwSwapBuffers(id_);
    glfwPollEvents();
}

double Window::getTime()
{
    return glfwGetTime();
}

float Window::getDeltaTime()
{
    float current = getTime();
    float dt = current - last_;
    last_ = current;
    return dt;
}


bool Window::isOpen() const
{
    return !glfwWindowShouldClose(id_);
}

