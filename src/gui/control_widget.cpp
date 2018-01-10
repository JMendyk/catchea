/**
 * @file control_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "control_widget.h"



ControlWidget* ControlWidget__create() {
    ControlWidget* cw = (ControlWidget*) malloc(sizeof(ControlWidget));
    if(!cw) return cw;

    return cw;
}

bool ControlWidget__init(ControlWidget* cw, void* app) {
    return true;
}

void ControlWidget__render(ControlWidget* cw, const ImVec2& window_pos, const ImVec2& window_size) {
    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Control Widget", NULL, window_flags);
    {
        ImGui::SetWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetWindowSize(window_size, ImGuiCond_Always);
    }
    ImGui::End();
}

bool ControlWidget__terminate(ControlWidget* cw) {
    return true;
}

void ControlWidget__destroy(ControlWidget* cw) {
    free(cw);
}
