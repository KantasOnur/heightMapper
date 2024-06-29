#include "Input.h"
#include "../Game.h"

bool Input::isKeyPressed(const int keyCode)
{
    return glfwGetKey(Game::getWindow().getPtr(), keyCode) == GLFW_PRESS;
}

bool Input::isMouseClicked(const int keyCode)
{
    return glfwGetMouseButton(Game::getWindow().getPtr(), keyCode) == GLFW_PRESS;
}

glm::vec2 Input::getMousePos()
{
    double xpos, ypos;
    glfwGetCursorPos(Game::getWindow().getPtr(), &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}
