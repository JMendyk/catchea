/**
 * @file footer_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "footer_widget.h"

#include <imgui_internal.h>



FooterWidget* FooterWidget__create() {
    FooterWidget* fw = (FooterWidget*) malloc(sizeof(FooterWidget));
    if(!fw) return fw;

    return fw;
}

bool FooterWidget__init(FooterWidget* fw, App* app) {
    return true;
}

void FooterWidget__render(FooterWidget* fw, const ImVec2& window_pos, const ImVec2& window_size) {
    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Footer Widget", NULL, window_flags);
    {
        ImGui::SetWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetWindowSize(window_size, ImGuiCond_Always);

        for (int idx = 0; idx < 1; idx++) {
            if (idx == 0) {
                ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            }
            ImGui::SameLine();
        }
    }
    ImGui::End();
}

bool FooterWidget__terminate(FooterWidget* fw) {
    return true;
}

void FooterWidget__destroy(FooterWidget* fw) {
    free(fw);
}
