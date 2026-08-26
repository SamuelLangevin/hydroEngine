#include "GuiManager.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cmath>
#include <iostream>
#include <string>
#include <gtc/type_ptr.hpp>

static const DirectionalWave DEFAULT_DIRECTIONAL_WAVE{glm::vec2(0.2f, 0.7f), 1.0f, 0.5f, 3.0};
static const PointWave DEFAULT_POINT_WAVE{glm::vec2(0.0f,0.0f), 0.0f, 3.0f, 5.0, 20.0f};

DirectionalWave GuiManager::directionalWave{glm::vec2(0.2f, 0.7f)};
glm::vec2 GuiManager::directionData{};
PointWave GuiManager::pointWave{glm::vec2(0.0f), 0.0f};
bool GuiManager::resetWaves = false;
GLFWwindow * GuiManager::window = nullptr;

void GuiManager::init(GLFWwindow * _window) {
    window = _window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    pointWave = DEFAULT_POINT_WAVE;
    directionalWave = DEFAULT_DIRECTIONAL_WAVE;
    directionData = DEFAULT_DIRECTIONAL_WAVE.getDirection();
}

void GuiManager::free() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::drawWaveParametersCollapsingHeader() {
    if (ImGui::CollapsingHeader("Wave parameters")) {
        ImGui::Text("Directional wave");
        ImGui::Separator();
        InputFloat(directionalWave.getWaveLength(), "Wave length", "##len1",
            [](float newValue) {directionalWave.setWaveLength(newValue);});
        InputFloat(directionalWave.getAmplitude(), "Amplitude", "##amp1",
            [](float newValue){directionalWave.setAmplitude(newValue);});
        InputFloat(directionalWave.getSpeed(), "Speed", "##spd1",
            [](float newValue){directionalWave.setSpeed(newValue);});
        if (ImGui::Button("Reset to default values##1")) directionalWave = DEFAULT_DIRECTIONAL_WAVE;
        InputVec2(directionData, "Direction", "##direct",
            [](glm::vec2 newDirection){directionalWave.setDirection(newDirection);});

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("Point wave");
        ImGui::Separator();
        InputFloat(pointWave.getWaveLength(), "Wave length", "##len2",
            [](float newValue){pointWave.setWaveLength(newValue);});
        InputFloat(pointWave.getAmplitude(), "Amplitude", "##amp2",
            [](float newValue){pointWave.setAmplitude(newValue);});
        InputFloat(pointWave.getSpeed(), "Speed", "##spd2",
            [](float newValue){pointWave.setSpeed(newValue);});
        if (ImGui::Button("Reset to default values##2")) pointWave = DEFAULT_POINT_WAVE;
        resetWaves = ImGui::Button("Clear waves");
    }
}

void GuiManager::draw() {
    ImGuiInputTextFlags textFlags = ImGuiInputTextFlags_EnterReturnsTrue;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Menu")) {
        if (ImGui::MenuItem("Exit")) {
            glfwSetWindowShouldClose(window, true);
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    ImGui::Begin("Simulation - Press escape to interact");
    drawWaveParametersCollapsingHeader();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiManager::setCaptureInput(bool capture) {
    ImGuiIO& io = ImGui::GetIO();
    io.SetAppAcceptingEvents(capture);
}

void GuiManager::InputText(std::string & text, const std::string & label, ImGuiInputTextFlags textFlags) {
    char * buf = text.data();
    ImGui::TextUnformatted(label.c_str());
    ImGui::SameLine();
    bool input = ImGui::InputText(("##" + label).c_str(), buf, 20, textFlags);
    std::string str(buf);
    if (input) text = std::string(buf);
}

void GuiManager::InputFloat(float value, const char * label, const char * id, const std::function<void(float)>& setter) {
    ImGui::TextUnformatted(label);
    ImGui::SameLine();
    if (ImGui::InputFloat(id, &value)) setter(value);
}

void GuiManager::InputVec2(glm::vec2 & values, const char * label, const char * id, const std::function<void(glm::vec2)>& setter) {
    ImGui::TextUnformatted(label);
    ImGui::SameLine();
    if (ImGui::InputFloat2(id,glm::value_ptr(values))) setter(values);
}



