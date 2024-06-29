#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
class Input
{
public:
    static bool isKeyPressed(const int keyCode);
    static bool isMouseClicked(const int keyCode);
    static glm::vec2 getMousePos();
};

#endif //INPUT_H
