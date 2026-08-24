#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>

#include "utility/Waves.hpp"


class GuiManager {
    public:
        DirectionalWave directionalWave{glm::vec2(0.2f, 0.7f)};
        PointWave pointWave{glm::vec2(0.0f), 0.0f};

        GuiManager() = default;
        ~GuiManager() = default;

        void init(GLFWwindow * _window);
        void free();
        void draw();
        void setCaptureInput(bool capture);

        static void InputText(std::string & text, const std::string &label, ImGuiInputTextFlags textFlags);
        static void InputFloat(float & initialValue, const std::string & label, const std::string & id);
        static void InputVec2(glm::vec2 & values, const std::string & label, const std::string & id, bool normalize);

    private:
        GLFWwindow * window = nullptr;
};


#endif