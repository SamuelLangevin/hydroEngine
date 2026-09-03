#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <imgui.h>
#include <string>
#include "../../includes/glad.h"
#include <GLFW/glfw3.h>

#include "../utility/Waves.hpp"

/** \class GuiManager
 * Manages ImGui's state and renders its windows.
 */
class GuiManager {
    private:

        static GLFWwindow * window; /**< A pointer on the application's window. */
        static int selectedDirectionalWave; /**< The index corresponding to the selected directional wave. =-1 if none is selected. */
        static glm::vec2 directionData; /**< Displayed direction value to not normalize directly the user's input. */
        static PointWave pointWaveParameters; /**< The selected parameters to generate point waves.*/

    public:
        static const DirectionalWave DEFAULT_DIRECTIONAL_WAVE; /**< The default parameters when adding/resetting a directional wave. */
        static const PointWave DEFAULT_POINT_WAVE; /**< The default parameters when adding/resetting a point wave. */

        GuiManager() = delete;

        /**
         * Initializes ImGui's state, sets the flags and default values.
         * @param _window object of the app class
         */
        static void init(GLFWwindow * _window);

        /** Shuts down ImGui's interface. */
        static void free();

        static PointWave getPointWaveParameters();

        /** Tells whether ImGui should capture the user's input. */
        static void setCaptureInput(bool capture);

        /** Draws ImGui's windows. */
        static void draw();

        /** Draws tha directional wave list collapsable */
        static void drawDirectionalWaveList();

        /** Draws the inputs to modify a directional wave's attributes. */
        static void showADirWaveProperties();

        /** Draws the point wave parameters collapsable. */
        static void drawPointWaveParameters();

        /**
         * @param wave directional or point wave to modify
         * @param imGuiID
         */
        static void showAWaveParameters(Wave * wave, int imGuiID);

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
         * @param value to display
         * @param label the name of this value
         * @param id for ImGui's management
         * @returns true if the user finished inputting and its input value
         */
        static std::pair<bool, float> InputFloat(float value, const char * label, const char * id);

        /**
         * Displays a text and a 2D vector input on the same line.
         * @param values to use and modify
         * @param label the name of this vector
         * @param id for ImGui's management
         * @returns true if the user clicked the set button and the input values
         */
        static std::pair<bool, glm::vec2> InputVec2(glm::vec2 & values, const char * label, const char * id);

        /**
         * Adds vertical spacing x times
         * @param times
         */
        static void SpacingTimes(uint times);
};


#endif