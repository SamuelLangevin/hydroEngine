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

void GuiManager::init(GLFWwindow * _window) {
    this->window = _window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    pointWave = DEFAULT_POINT_WAVE;
    directionalWave = DEFAULT_DIRECTIONAL_WAVE;
}

void GuiManager::free() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

    ImGui::Begin("Simulation");
    if (ImGui::CollapsingHeader("Wave parameters")) {
        ImGui::Text("Directional wave");
        ImGui::Separator();
        InputFloat(directionalWave.waveLength, "Wave length", "##len1");
        InputFloat(directionalWave.magnitude, "Magnitude", "##mag1");
        InputFloat(directionalWave.speed, "Speed", "##spd1");
        if (ImGui::Button("Reset to default values##1")) directionalWave = DEFAULT_DIRECTIONAL_WAVE;
        InputVec2(directionalWave.direction, "Direction", "##direct");

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("Point wave");
        ImGui::Separator();
        InputFloat(pointWave.waveLength, "Wave length", "##len2");
        InputFloat(pointWave.magnitude, "Magnitude", "##mag2");
        InputFloat(pointWave.speed, "Speed", "##spd2");
        if (ImGui::Button("Reset to default values##2")) pointWave = DEFAULT_POINT_WAVE;
    }
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

void GuiManager::InputFloat(float & value, const char * label, const char * id) {
    ImGui::TextUnformatted(label);
    ImGui::SameLine();
    ImGui::InputFloat(id, &value);
}

void GuiManager::InputVec2(glm::vec2 & values, const char * label, const char * id) {
    ImGui::TextUnformatted(label);
    ImGui::SameLine();
    ImGui::InputFloat2(id,glm::value_ptr(values));
}
