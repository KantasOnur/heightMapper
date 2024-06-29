#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow* id_;
    int width_;
    int height_;
public:
    Window(int width, int height);
    ~Window();

    void update() const;
    double getTime();
    bool isOpen() const;

};

#endif //WINDOW_H
