#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>

#include "utility/Waves.hpp"

/** \class GuiManager
 * Manages ImGui's state and renders its windows.
 */
class GuiManager {
    public:
        DirectionalWave directionalWave{glm::vec2(0.2f, 0.7f)}; /**< Stores the parameters the user has set for directional waves.*/
        PointWave pointWave{glm::vec2(0.0f), 0.0f}; /**< Stores the parameters the user has set for point waves.*/

        GuiManager() = default;
        ~GuiManager() = default;

        /**
         * Initializes ImGui's state, sets the flags and default values.
         * @param _window
         */
        void init(GLFWwindow * _window);

        /** Shuts down ImGui's interface. */
        void free();

        /** Draws ImGui's windows. */
        void draw();

        /** Tells whether ImGui should capture the user's input. */
        void setCaptureInput(bool capture);

        /**
         * Displays a text and a text input on the same line.
         * @param text to use and modify
         * @param label The name of the text to modify and its Imgui ID.
         * @param textFlags
         *
         * fixme the app crashes if the string is too long.
         */
        static void InputText(std::string & text, const std::string &label, ImGuiInputTextFlags textFlags);

        /**
         * Displays a text and a float input on the same line.
         * @param value to use and modify
         * @param label the name of this value
         * @param id for ImGui's management
         */
        static void InputFloat(float & value, const char * label, const char * id);

        /**
         * Displays a text and a 2D vector input on the same line.
         * @param values to use and modify
         * @param label the name of this vector
         * @param id for ImGui's management
         */
        static void InputVec2(glm::vec2 & values, const char * label, const char * id);

    private:
        GLFWwindow * window = nullptr; /**< A pointer on the application's window. */
};


#endif