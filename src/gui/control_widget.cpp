/**
 * @file control_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "control_widget.h"

#include <vector>
#include <algorithm>
#include <dis_tile/dis_tile.h>
#include <imgui_internal.h>
#include <dis_interpreters/catchmenter.h>

#include "geo_tile/geo_tile.h"
#include "dis_tile/dis_tile.h"

#include "app.h"

#include "map_widget.h"


ControlWidget* ControlWidget__create() {
    ControlWidget* cw = (ControlWidget*) malloc(sizeof(ControlWidget));
    if (!cw) return cw;

    return cw;
}

bool ControlWidget__init(ControlWidget* cw, App* app) {

    cw->app = app;

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


        static const RealTile::Coloring steps_lower = {000, 000, 000, 255};
        static const RealTile::Coloring steps_upper = {000, 000, 000, 255};

        static const std::vector<std::pair<int, RealTile::Coloring>> preset_color = {
                { 0000, {000, 000, 255, 255} },
                { 0000, {000, 255, 000, 255} },
                { 0300, {255, 255, 000, 255} },
                { 2000, {255, 000, 000, 255} },
        };

        static const std::vector<std::pair<int, RealTile::Coloring>> preset_black_and_white = {
                { 0000, { 000, 000, 000, 255 } },
                { 2000, { 255, 255, 255, 255 } },
        };

        static std::vector<std::pair<int, RealTile::Coloring>> steps = preset_color;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                            ImVec2(ImGui::GetStyle().ItemSpacing.x, 2 * ImGui::GetStyle().ItemSpacing.y));

        ImGui::Text("Elevation Coloring");

        ImGui::Columns(5, NULL, false);
        ImGui::SetColumnWidth(0, 29);
        ImGui::SetColumnWidth(1, 29);
        ImGui::SetColumnWidth(2, 29);

        float column_margin = 2 * ImGui::GetStyle().ItemSpacing.x;

        typedef std::vector<std::pair<int, RealTile::Coloring>>::iterator iter;
        iter prev, next;


        for (auto step = steps.begin(); steps.size() > 0 && step != steps.end(); step++) {
            float total_width = 0;

            ImGui::PushID((int) std::distance(steps.begin(), step));
            ImGui::BeginGroup();

            if (step != steps.begin()) {
                if (ImGui::Button("/\\", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
                    prev = step - 1;
                    next = step;
                    std::swap(prev->first, next->first);
                    iter_swap(prev, next);
                }
            }
            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            if (step != steps.end() - 1) {
                if (ImGui::Button("\\/", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
                    prev = step;
                    next = step + 1;
                    std::swap(prev->first, next->first);
                    iter_swap(prev, next);
                }
            }
            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            {
                RealTile::Coloring dts = step->second;
                float sc = 1.0f / 255.0f;
                float color[3] = {sc * dts.red, sc * dts.green, sc * dts.blue};
                ImGui::PushItemWidth(ImGui::GetColumnWidth()/* - column_margin*/);
                ImGui::ColorEdit3("", color, ImGuiColorEditFlags_NoInputs);
                step->second.red = static_cast<unsigned char>(color[0] / sc);
                step->second.green = static_cast<unsigned char>(color[1] / sc);
                step->second.blue = static_cast<unsigned char>(color[2] / sc);
            }

            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            {
                int x = step->first;
                int v_min = step == steps.begin() ? x - 1000 : (step - 1)->first;
                int v_max = step + 1 == steps.end() ? x + 1000 : (step + 1)->first;

                ImGui::PushItemWidth(ImGui::GetColumnWidth() - column_margin / 2);
                ImGui::DragInt("2", &step->first, 1.0f, v_min, v_max);

                step->first = x;
            }

            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            if (ImGui::Button("X", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
                steps.erase(step);
            }

            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            ImGui::EndGroup();
            ImGui::PopID();

            ImGui::SetColumnWidth(3, total_width - 4 * 29 - column_margin / 2);

        }

        ImGui::Columns(1);

        if (ImGui::Button("Add")) {
            steps.insert(steps.end(),
                         {(steps.end() - 1)->first, {000, 000, 000, 255}});
        }

        if (ImGui::Button("Apply changes")) {
            MapWidget__update_tile2(cw->app->mapWidget, steps_lower, steps_upper, steps);
        }

        ImGui::Separator();

        bool catchmented = false;
        srand(1);

        ImGui::Text("Catchmenting with K4");

        ImGui::Columns(2);
        ImGui::PushID("catchmenting_k4");

        if (ImGui::Button("Normal", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all(cw->app->realTile, K4);
        }
        ImGui::NextColumn();
        if (ImGui::Button("Immediate", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate(cw->app->realTile, K4);
        }
        ImGui::NextColumn();
        if (ImGui::Button("HM Normal", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all(cw->app->realTile, K4_HARD_MIN);
        }
        ImGui::NextColumn();
        if (ImGui::Button("HM Immediate", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate(cw->app->realTile, K4_HARD_MIN);
        }
        ImGui::NextColumn();
        if (ImGui::Button("Heightwise", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K4);
        }
        ImGui::NextColumn();
        if (ImGui::Button("HM Heightwise", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K4_HARD_MIN);
        }
        ImGui::NextColumn();

        ImGui::PopID();

        ImGui::Separator();

        ImGui::Columns(1);
        ImGui::Text("Catchmenting with K8");

        ImGui::Columns(2);
        ImGui::PushID("catchmenting_k8");

        if (ImGui::Button("Normal", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all(cw->app->realTile, K8);
        }
        ImGui::NextColumn();
        if (ImGui::Button("Immediate", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate(cw->app->realTile, K8);
        }
        ImGui::NextColumn();
        if (ImGui::Button("HM Normal", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all(cw->app->realTile, K8_HARD_MIN);
        }
        ImGui::NextColumn();
        if (ImGui::Button("HM Immediate", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate(cw->app->realTile, K8_HARD_MIN);
        }
        ImGui::NextColumn();
        if (ImGui::Button("Heightwise", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K8);
        }
        ImGui::NextColumn();
        if (ImGui::Button("HM Heightwise", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K8_HARD_MIN);
        }
        ImGui::NextColumn();
        if (ImGui::Button("All", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__all(cw->app->realTile, K8);
        }
        ImGui::NextColumn();
        if (ImGui::Button(" HM All", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            catchmented = true;
            Catchmenter__all(cw->app->realTile, K8_HARD_MIN);
        }
        ImGui::NextColumn();

        ImGui::PopID();

        if (catchmented) {
            RealTile__texture_generate(cw->app->realTile);
        }

        ImGui::PopStyleVar();

    }
    ImGui::End();
}

bool ControlWidget__terminate(ControlWidget* cw) {
    return true;
}

void ControlWidget__destroy(ControlWidget* cw) {
    free(cw);
}
