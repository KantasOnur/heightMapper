#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

struct windowParams
{
    float width;
    float height;
};

class Window
{
private:
    GLFWwindow* id_;
    float width_;
    float height_;
    float last_ = 0.0f;
public:
    Window(int width, int height);
    ~Window();

    void update() const;
    double getTime();
    float getDeltaTime();
    bool isOpen() const;
    float getAspectRatio() const {return width_ / height_;}
    GLFWwindow* getPtr() const {return id_;}
    windowParams getParams() const {return {width_, height_};}
};

#endif //WINDOW_H
