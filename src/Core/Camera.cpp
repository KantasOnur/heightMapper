#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtx/string_cast.hpp>
#include <complex>
#include "../Game.h"

Camera::Camera(float fov, glm::vec3 position, glm::vec3 up, glm::vec3 initialLookDir)
    : position_(position), up_(up), front_(initialLookDir),
    yaw_(glm::degrees(atan2(initialLookDir.z, initialLookDir.x))),
    pitch_(atan2(initialLookDir.y, sqrt(initialLookDir.x * initialLookDir.x + initialLookDir.z * initialLookDir.z))),
    fov_(fov), initialLookDir_(initialLookDir)
{
    updateProjectionMatrix();
}

void Camera::updatePosition(Input &input, float dt)
{
    /*This calulcation ensures that the up vector is always perpendicular to the front vector.*/

    /*
    const float projection = glm::dot(up_, front_);
    const glm::vec3 projectionFront = front_ * projection;
    const glm::vec3 upPerpendicular = up_ - projectionFront;
    */
    //up_ = glm::normalize(upPerpendicular);
    right_ = glm::normalize(glm::cross(front_, up_));

    glm::vec3 movementVector = glm::vec3(0.0f, 0.0f, 0.0f);
    const float speed = 2.5f * dt;
    if(input.isKeyPressed(GLFW_KEY_W))
        movementVector += front_;
    if(input.isKeyPressed(GLFW_KEY_S))
        movementVector -= front_;
    if(input.isKeyPressed(GLFW_KEY_D))
        movementVector += right_;
    if(input.isKeyPressed(GLFW_KEY_A))
        movementVector -= right_;

    if(input.isKeyPressed(GLFW_KEY_SPACE))
        movementVector += up_;
    if(input.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        movementVector -= up_;

    if(input.isKeyPressed(GLFW_KEY_ESCAPE))
        initialClick = false;

    if(movementVector != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        position_ += glm::normalize(movementVector) * speed;
    }
    viewM_ = glm::lookAt(position_,  position_ + front_, up_);
}

void Camera::updateLookDir(Input &input, float dt)
{
    initialClick = input.isKeyPressed(GLFW_KEY_TAB) ? true : initialClick;
    if(initialClick)
    {
        GLFWwindow* windowPtr = Game::getWindow().getPtr();

        glm::vec2 mousePos = input.getMousePos();
        windowParams params = Game::getWindow().getParams();
        glm::vec2 deltaPos = mousePos - glm::vec2(params.width/2, params.height/2);

        float speed = 10.0f * dt;
        yaw_ += speed * deltaPos.x/2;
        pitch_ -= speed * deltaPos.y/2;

        float maxPitch = 89.0f;
        pitch_ = glm::clamp(pitch_, -maxPitch, maxPitch);

        glm::vec3 front;
        front.x = (cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)));
        front.y = sin(glm::radians(pitch_));
        front.z = (sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)));

        front_ = glm::normalize(front);
        glfwSetCursorPos(windowPtr, params.width/2, params.height/2);
        glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        Game::openGui = false;
    }
    else
    {
        glfwSetInputMode(Game::getWindow().getPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Game::openGui = true;
    }
}

void Camera::update(float dt)
{
    Input& input = Game::getInput();

    updateLookDir(input, dt);
    updatePosition(input, dt);
}

void Camera::updateProjectionMatrix()
{
    projM_ = glm::perspective(fov_, Game::getWindow().getAspectRatio(), 0.1f, 100.0f);
}
