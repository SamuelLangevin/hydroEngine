#include "GuiManager.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cmath>
#include <iostream>
#include <string>
#include <gtc/type_ptr.hpp>

const DirectionalWave GuiManager::DEFAULT_DIRECTIONAL_WAVE{glm::vec2(0.2f, 0.7f), 4.0f, 1.5f, 3.0};
const PointWave GuiManager::DEFAULT_POINT_WAVE{glm::vec2(0.0f,0.0f), 0.0f, 3.0f, 2.0, 20.0f};

GLFWwindow * GuiManager::window = nullptr;
std::vector<DirectionalWave> * GuiManager::directionalWaves = nullptr;
std::vector<PointWave> * GuiManager::pointWaves = nullptr;
int GuiManager::selectedDirectionalWave = -1;
PointWave * GuiManager::selectedPointWaveParameters = nullptr;

glm::vec2 GuiManager::directionData{};
bool GuiManager::resetWaves = false;

void GuiManager::init(GLFWwindow * _window, std::vector<DirectionalWave> * dirWaves, std::vector<PointWave> * ptWaves, PointWave * pointWaveParameters) {
    window = _window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    directionData = DEFAULT_DIRECTIONAL_WAVE.getDirection();
    directionalWaves = dirWaves;
    pointWaves = ptWaves;
    selectedPointWaveParameters = pointWaveParameters;
}

void GuiManager::free() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::setCaptureInput(bool capture) {
    ImGuiIO& io = ImGui::GetIO();
    io.SetAppAcceptingEvents(capture);
}

void GuiManager::draw() {
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
    drawDirectionalWaveList();
    drawPointWaveParameters();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiManager::drawDirectionalWaveList() {

    ImGuiTreeNodeFlags treeFlag = ImGuiTreeNodeFlags_DefaultOpen, ImGuiTreeNodeFlags_Leaf, ImGuiTreeNodeFlags_OpenOnArrow;
    if (ImGui::CollapsingHeader("Directional waves")) {
        if (ImGui::Button("Add wave")) directionalWaves->push_back(DEFAULT_DIRECTIONAL_WAVE);
        ImGui::SameLine();
        if (ImGui::Button("Clear waves##1")) {
            directionalWaves->clear();
            selectedDirectionalWave = -1;
        }

        ImGui::BeginChild("ChildList", ImVec2(ImGui::GetContentRegionAvail().x, 100), ImGuiChildFlags_None);
        if (ImGui::TreeNodeEx("Waves", treeFlag)) {

            for (int i = 0; i < directionalWaves->size(); i++) {
                bool isClicked = ImGui::Selectable(("DWave " + std::to_string(i + 1)).c_str(), i == selectedDirectionalWave);
                if (isClicked) selectedDirectionalWave = (i == selectedDirectionalWave ? -1 : i);
            }
            ImGui::TreePop();
        }
        ImGui::EndChild();

        ImGui::BeginGroup();
        if (0 <= selectedDirectionalWave && selectedDirectionalWave < directionalWaves->size()) showADirWaveProperties();
        ImGui::EndGroup();
        SpacingTimes(3);
    }
}

void GuiManager::showADirWaveProperties() {
    ImGui::SeparatorText("Inspector");
    ImGui::TextUnformatted(("Directional wave " + std::to_string(selectedDirectionalWave + 1)).c_str());
    ImGui::Separator();
    DirectionalWave & wave = directionalWaves->at(selectedDirectionalWave);

    if (ImGui::Button("Reset to default values##1")) wave = DEFAULT_DIRECTIONAL_WAVE;
    showAWaveParameters(&wave, 1);

    static int previousSelected = -1;
    if (previousSelected != selectedDirectionalWave) directionData = wave.getDirection();
    previousSelected = selectedDirectionalWave;
    auto [enteredDirection, directionValue] = InputVec2(directionData, "Set direction", "##dirct1");
    if (enteredDirection) wave.setDirection(directionValue);

    SpacingTimes(2);

    if (ImGui::Button("Delete wave")) {
        directionalWaves->erase(directionalWaves->begin() + selectedDirectionalWave);
        selectedDirectionalWave = -1;
    }
    ImGui::SeparatorText("");
}

void GuiManager::drawPointWaveParameters() {
    if (ImGui::CollapsingHeader("Point waves")) {
        if (ImGui::Button("Reset to default values##2")) *selectedPointWaveParameters = DEFAULT_POINT_WAVE;
        ImGui::SameLine();
        if (ImGui::Button("Clear waves##2")) pointWaves->clear();
        showAWaveParameters(selectedPointWaveParameters, 2);
        SpacingTimes(3);
    }
}

void GuiManager::showAWaveParameters(Wave * wave, int imGuiID) {
    auto [enteredLength, lengthValue] = InputFloat(wave->getWaveLength(), "Wave length", ("##len" + std::to_string(imGuiID)).c_str());
    if (enteredLength) wave->setWaveLength(lengthValue);
    auto [enteredAmpl, amplValue] = InputFloat(wave->getAmplitude(), "Amplitude", ("##amp" + std::to_string(imGuiID)).c_str());
    if (enteredAmpl) wave->setAmplitude(amplValue);
    auto [enteredSpeed, speedValue] = InputFloat(wave->getSpeed(), "Speed", ("##spd" + std::to_string(imGuiID)).c_str());
    if (enteredSpeed) wave->setSpeed(speedValue);
}

void GuiManager::InputText(std::string & text, const std::string & label, ImGuiInputTextFlags textFlags) {
    char * buf = text.data();
    ImGui::TextUnformatted(label.c_str());
    ImGui::SameLine();
    bool input = ImGui::InputText(("##" + label).c_str(), buf, 20, textFlags);
    std::string str(buf);
    if (input) text = std::string(buf);
}

std::pair<bool, float> GuiManager::InputFloat(float value, const char * label, const char * id) {
    ImGui::TextUnformatted(label);
    ImGui::SameLine();
    bool entered = ImGui::InputFloat(id, &value);
    return {entered, value};
}

std::pair<bool, glm::vec2> GuiManager::InputVec2(glm::vec2 & values, const char * label, const char * id) {
    bool entered = ImGui::Button((std::string(label)).c_str());
    ImGui::SameLine();
    ImGui::InputFloat2(id,glm::value_ptr(values));
    return {entered, values};
}

void GuiManager::SpacingTimes(uint times) {
    for (uint i = 0; i < times; i++) {
        ImGui::Spacing();
    }
}



