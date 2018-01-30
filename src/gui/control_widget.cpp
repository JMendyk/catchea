/**
 * @file control_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "control_widget.h"

#include <vector>
#include <algorithm>
#include <imgui_internal.h>
#include <dis_interpreters/catchmenter.h>
#include <dis_interpreters/topographer.h>

#include "app.h"

#include "map_widget.h"


ControlWidget* ControlWidget__create() {
    ControlWidget* cw = (ControlWidget*) malloc(sizeof(ControlWidget));
    if (!cw) return cw;

    cw->jumpMax = 1;

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

        float column_margin = 2 * ImGui::GetStyle().ItemSpacing.x;

        static std::vector<RealTileSample> steps = REALTILE_PRESET_COLOR;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                            ImVec2(ImGui::GetStyle().ItemSpacing.x, 2 * ImGui::GetStyle().ItemSpacing.y));

        ImGui::Text(1, "Elevation Coloring");
        ImGui::Separator();

        ImGui::Text("Presets:");
        ImGui::Columns(2, NULL, false);

        if (ImGui::Button("Black and White", ImVec2(ImGui::GetColumnWidth() - column_margin/2, 0))) {
            steps = REALTILE_PRESET_BLACK_AND_WHITE;
        }
        ImGui::NextColumn();
        if (ImGui::Button("Standard Colors", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            steps = REALTILE_PRESET_COLOR;
        }

        ImGui::Columns(1);
        ImGui::Text("Customization:");

        ImGui::Columns(5, NULL, false);
        ImGui::SetColumnWidth(0, 29);
        ImGui::SetColumnWidth(1, 29);
        ImGui::SetColumnWidth(2, 29);

        typedef std::vector<RealTileSample>::iterator iter;
        iter prev, next;

        for (iter step = steps.begin(); steps.size() > 0 && step < steps.end(); step++) {
            float total_width = 0;

            ImGui::PushID((int) std::distance(steps.begin(), step));
            ImGui::BeginGroup();

            if (step != steps.begin()) {
                if (ImGui::Button("/\\", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
                    prev = step - 1;
                    next = step;
                    std::swap(prev->height, next->height);
                    iter_swap(prev, next);
                }
            }
            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            if (step != steps.end() - 1) {
                if (ImGui::Button("\\/", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
                    prev = step;
                    next = step + 1;
                    std::swap(prev->height, next->height);
                    iter_swap(prev, next);
                }
            }
            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            {
                RealTile::Coloring dts = step->coloring;
                float sc = 1.0f / 255.0f;
                float color[3] = {sc * dts.red, sc * dts.green, sc * dts.blue};
                ImGui::PushItemWidth(ImGui::GetColumnWidth()/* - column_margin*/);
                ImGui::ColorEdit3("", color, ImGuiColorEditFlags_NoInputs);
                step->coloring.red =   (unsigned char) (color[0] / sc);
                step->coloring.green = (unsigned char) (color[1] / sc);
                step->coloring.blue =  (unsigned char) (color[2] / sc);
            }

            total_width += ImGui::GetColumnWidth();
            ImGui::NextColumn();

            {
                int x = step->height;
                int v_min = step == steps.begin()   ? (x - 1000) : (step - 1)->height;
                int v_max = step + 1 == steps.end() ? (x + 1000) : (step + 1)->height;

                ImGui::PushItemWidth(ImGui::GetColumnWidth() - column_margin / 2);
                ImGui::DragInt("", &x, 1.0f, v_min, v_max, "%.0f m n.p.m.");

                step->height = x;
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

        ImGui::Columns(2, NULL, false);

        if (ImGui::Button("+", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
            steps.insert(steps.end(),
                         {(steps.end() - 1)->height, {000, 000, 000, 255}});
        }
        ImGui::NextColumn();

        if (ImGui::Button("Start coloring", ImVec2(ImGui::GetColumnWidth() - column_margin, 0))) {
            Topographer__interpret(cw->app->realTile, REALTILE_DEFAULT_COLORING_LOWER, REALTILE_DEFAULT_COLORING_UPPER, steps);
            RealTile__texture_generate(cw->app->realTile);
        }

        ImGui::Separator();
        ImGui::Separator();

        ImGui::Columns(1);

        bool catchmented = false;
        srand(1);

        ImGui::Text(1, "Catchment area generation");
        ImGui::Separator();

        if (ImGui::Button("K4 Immediate", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate(cw->app->realTile, K4_HARD_MIN);
        }
        if (ImGui::Button("K4 Heightwise", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K4_HARD_MIN);
        }
        if (ImGui::Button("K8 Immediate", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate(cw->app->realTile, K8_HARD_MIN);
        }
        if (ImGui::Button("K8 Heightwise", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
            catchmented = true;
            Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K8_HARD_MIN);
        }
        if (ImGui::Button("K8 Disjunctive", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
            catchmented = true;
            Catchmenter__all(cw->app->realTile, K8_HARD_MIN, cw->jumpMax);
        }

        if (catchmented) {
            RealTile__texture_generate(cw->app->realTile);
        }

        ImGui::InputInt("Coloring jump", &cw->jumpMax, 1);
        if(cw->jumpMax < 1) cw->jumpMax = 1;

        ImGui::Separator();
        ImGui::Separator();

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
