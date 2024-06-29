#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

class Camera
{
private:
    glm::vec3 position_, front_, up_, right_;
    glm::mat4 projM_, viewM_;
    float pitch_ = 0.0f, yaw_ = 0.0f;
    float fov_, aspectRatio_;
    glm::vec3 initialLookDir_;

    bool initialClick = false;

    void updateLookDir(Input &input, float dt);
    void updatePosition(Input &input, float dt);
    void updateProjectionMatrix();
public:
    Camera(float fov = 45.0f, glm::vec3 position = {0.0f, 0.0f, 0.0f}, glm::vec3 up = {0.0f, 1.0f, 0.0f},
        glm::vec3 initialLookDir = {0.0f, 0.0f, -1.0f});
    glm::mat4 getProjection() const {return projM_;}
    glm::mat4 getView() const {return viewM_;}
    void update(float dt);
};
#endif //CAMERA_H
