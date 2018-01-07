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

bool FooterWidget__init(FooterWidget* fw) {
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

        /*
         * Solution based on an answer to ImGui issue #934
         * https://github.com/ocornut/imgui/issues/934#issuecomment-340231002
         * I (@JMendyk) refactored mentioned solution using a for-loop
         * thus order of elements is the same as they will appear on the screen
         */

        static const int elements_count = 4;

        static float widths[elements_count];
        static const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;
        float pos = 0;

        for (int idx = elements_count - 1; idx >= 0; idx--) {
            pos += widths[idx] + ItemSpacing;
            ImGui::SameLine(ImGui::GetWindowWidth() - pos);
            ImGui::BeginGroup();
            if (idx == 0) {
                ImGui::Text("Min: %d", -20);
                ImGui::SameLine();
                ImGui::VerticalSeparator();
            } else if (idx == 1) {
                ImGui::Text("Avg: %d", 67);
                ImGui::SameLine();
                ImGui::VerticalSeparator();
            } else if (idx == 2) {
                ImGui::Text("Max: %d", 2499);
                ImGui::SameLine();
                ImGui::VerticalSeparator();
            } else if (idx == 3) {
                ImGui::Text("File: %s", "file1.hgt");
            }
            ImGui::EndGroup();
            widths[idx] = ImGui::GetItemRectSize().x;
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
