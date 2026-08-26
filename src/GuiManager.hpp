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

        static DirectionalWave directionalWave; /**< Stores the parameters the user has set for directional waves.*/
        static PointWave pointWave; /**< Stores the parameters the user has set for point waves.*/
        static glm::vec2 directionData; /**< Displayed direction value to not normalize directly the user's input. */
        static bool resetWaves; /**< Tells if the application should clear waves. */

        GuiManager() = delete;

        /**
         * Initializes ImGui's state, sets the flags and default values.
         * @param _window
         */
        static void init(GLFWwindow * _window);

        /** Shuts down ImGui's interface. */
        static void free();

        /** Draws ImGui's windows. */
        static void draw();

        /** Draws the collapsing header for wave parameters. */
        static void drawWaveParametersCollapsingHeader();

        /** Tells whether ImGui should capture the user's input. */
        static void setCaptureInput(bool capture);

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
         * @param setter a function to call on input
         */
        static void InputFloat(float value, const char * label, const char * id, const std::function<void(float)>& setter);

        /**
         * Displays a text and a 2D vector input on the same line.
         * @param values to use and modify
         * @param label the name of this vector
         * @param id for ImGui's management
         * @param setter a function to call on input
         */
        static void InputVec2(glm::vec2 & values, const char * label, const char * id, const std::function<void(glm::vec2)>& setter);

    private:
        static GLFWwindow * window; /**< A pointer on the application's window. */
};


#endif