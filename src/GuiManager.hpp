#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <GLFW/glfw3.h>


class GuiManager {
    public:

        GuiManager() = default;
        ~GuiManager() = default;

        void init(GLFWwindow * _window);
        void free();
        void draw();
        void setCaptureInput(bool capture);
private:
    GLFWwindow * window = nullptr;
};


#endif