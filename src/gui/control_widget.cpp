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
    if(!cw) return cw;

    return cw;
}

bool ControlWidget__init(ControlWidget* cw, App* app) {

    cw->app = app;

    return true;
}

geo_sample_t max(geo_sample_t a, geo_sample_t b);
geo_sample_t min(geo_sample_t a, geo_sample_t b);

template<class T, class Compare>
constexpr const T& clamp( const T& v, const T& lo, const T& hi, Compare comp )
{
    return assert( !comp(hi, lo) ),
        comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    return clamp( v, lo, hi, std::less<T>() );
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

        {

            static const DisTileSample steps_lower = {000, 000, 000, 255};
            static const DisTileSample steps_upper = {000, 000, 000, 255};

            static std::vector<std::pair<DisTileSample, geo_sample_t> > steps = {
                //std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32768),
                //std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32767),
                std::make_pair((DisTileSample) {000, 000, 255, 255}, 0),
                std::make_pair((DisTileSample) {000, 255, 000, 255}, 0),
                std::make_pair((DisTileSample) {255, 255, 000, 255}, 300),
                std::make_pair((DisTileSample) {255, 000, 000, 255}, 2000),
                //std::make_pair((DisTileSample){ 000, 000, 000, 255 },   8000)
            };

            ImGui::Columns(5, NULL, false);
            ImGui::SetColumnWidth(0, 29);
            ImGui::SetColumnWidth(1, 29);
            ImGui::SetColumnWidth(2, 29);

            float column_margin = 2 * ImGui::GetStyle().ItemSpacing.x;

            bool swap_elems = false;
            typedef std::vector<std::pair<DisTileSample, geo_sample_t> >::iterator iter;
            iter prev, next;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                                ImVec2(ImGui::GetStyle().ItemSpacing.x, 2 * ImGui::GetStyle().ItemSpacing.y));

            for (auto step = steps.begin(); steps.size() > 0 && step != steps.end(); step++) {
                float total_width = 0;

                ImGui::PushID((int) std::distance(steps.begin(), step));
                ImGui::BeginGroup();

                if (step != steps.begin()) {
                    if (ImGui::Button("/\\", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
                        prev = step - 1;
                        next = step;
                        std::swap(prev->second, next->second);
                        iter_swap(prev, next);
                    }
                }
                total_width += ImGui::GetColumnWidth();
                ImGui::NextColumn();

                if (step != steps.end() - 1) {
                    if (ImGui::Button("\\/", ImVec2(ImGui::GetColumnWidth() - column_margin / 2, 0))) {
                        prev = step;
                        next = step + 1;
                        std::swap(prev->second, next->second);
                        iter_swap(prev, next);
                    }
                }
                total_width += ImGui::GetColumnWidth();
                ImGui::NextColumn();

                DisTileSample dts = step->first;
                float sc = 1.0f / 255.0f;
                float color[3] = {sc * dts.red, sc * dts.green, sc * dts.blue};
                ImGui::PushItemWidth(ImGui::GetColumnWidth()/* - column_margin*/);
                ImGui::ColorEdit3("", color, ImGuiColorEditFlags_NoInputs);
                step->first.red = static_cast<unsigned char>(color[0] / sc);
                step->first.green = static_cast<unsigned char>(color[1] / sc);
                step->first.blue = static_cast<unsigned char>(color[2] / sc);

                total_width += ImGui::GetColumnWidth();
                ImGui::NextColumn();

                int x = step->second;
                // @VERSION: 2
                geo_sample_t v_min = (short) (x - 1000);
                geo_sample_t v_max = (short) (x + 1000);

                if (step != steps.begin())
                    v_min = (step - 1)->second;

                if (step + 1 != steps.end())
                    v_max = (step + 1)->second;

                ImGui::PushItemWidth(ImGui::GetColumnWidth() - column_margin / 2);
                // @VERSION: 2.1
                //ImGui::SliderInt("1", &x, v_min, v_max);
                // @VERSION: 2.2
                ImGui::DragInt("2", &x, 1.0f, v_min, v_max);

                // @VERSION: 2
                step->second = static_cast<geo_sample_t>(x);

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
            ImGui::PopStyleVar();

            ImGui::Columns(1);

            if (ImGui::Button("Add")) {
                steps.insert(steps.end(),
                             std::make_pair((DisTileSample) {000, 000, 000, 255}, (steps.end() - 1)->second));
            }

            if (ImGui::Button("Apply changes")) {
                MapWidget__update_tile(cw->app->mapWidget, steps_lower, steps_upper, steps);
            }

            ImGui::Separator();

            bool catchmented = false;
            srand(1);

            if (ImGui::Button("Catch4")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->geoTile, cw->app->disTile, K4);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch4 immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->geoTile, cw->app->disTile, K4);
            }
            if (ImGui::Button("Catch4 HM")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->geoTile, cw->app->disTile, K4_HARD_MIN);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch4 HM immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->geoTile, cw->app->disTile, K4_HARD_MIN);
            }

            ImGui::Separator();

            if (ImGui::Button("Catch8")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->geoTile, cw->app->disTile, K8);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch8 immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->geoTile, cw->app->disTile, K8);
            }
            if (ImGui::Button("Catch8 HM")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->geoTile, cw->app->disTile, K8_HARD_MIN);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch8 HM immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->geoTile, cw->app->disTile, K8_HARD_MIN);
            }

            if (catchmented) {
                rm_free_texture(cw->app->mapWidget->texTile);
                cw->app->mapWidget->texTile = DisTile__to_texture(cw->app->disTile);
            }

        }

        ImGui::Separator();

        ImGui::PushID("test123");
        {

            static const RealTile::Data steps_lower = {000, 000, 000, 255};
            static const RealTile::Data steps_upper = {000, 000, 000, 255};

            static std::vector<std::pair<int, RealTile::Data>> steps = {
                { 0, {000, 000, 255, 255} },
                { 0, {000, 255, 000, 255} },
                { 300, {255, 255, 000, 255} },
                { 2000, {255, 000, 000, 255} },
            };

            ImGui::Columns(5, NULL, false);
            ImGui::SetColumnWidth(0, 29);
            ImGui::SetColumnWidth(1, 29);
            ImGui::SetColumnWidth(2, 29);

            float column_margin = 2 * ImGui::GetStyle().ItemSpacing.x;

            bool swap_elems = false;
            typedef std::vector<std::pair<int, RealTile::Data>>::iterator iter;
            iter prev, next;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                                ImVec2(ImGui::GetStyle().ItemSpacing.x, 2 * ImGui::GetStyle().ItemSpacing.y));

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

                RealTile::Data dts = step->second;
                float sc = 1.0f / 255.0f;
                float color[3] = {sc * dts.red, sc * dts.green, sc * dts.blue};
                ImGui::PushItemWidth(ImGui::GetColumnWidth()/* - column_margin*/);
                ImGui::ColorEdit3("", color, ImGuiColorEditFlags_NoInputs);
                step->second.red = static_cast<unsigned char>(color[0] / sc);
                step->second.green = static_cast<unsigned char>(color[1] / sc);
                step->second.blue = static_cast<unsigned char>(color[2] / sc);

                total_width += ImGui::GetColumnWidth();
                ImGui::NextColumn();

                int x = step->first;
                // @VERSION: 2
                int v_min = x - 1000;
                int v_max = x + 1000;

                if (step != steps.begin())
                    v_min = (step - 1)->first;

                if (step + 1 != steps.end())
                    v_max = (step + 1)->first;

                ImGui::PushItemWidth(ImGui::GetColumnWidth() - column_margin / 2);
                // @VERSION: 2.1
                //ImGui::SliderInt("1", &x, v_min, v_max);
                // @VERSION: 2.2
                ImGui::DragInt("2", &x, 1.0f, v_min, v_max);

                // @VERSION: 2
                step->first = x;

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
            ImGui::PopStyleVar();

            ImGui::Columns(1);

            if (ImGui::Button("Add")) {
                steps.insert(steps.end(),
                             { (steps.end() - 1)->first, {000, 000, 000, 255} });
            }

            if (ImGui::Button("Apply changes")) {
                MapWidget__update_tile2(cw->app->mapWidget, steps_lower, steps_upper, steps);
            }

            ImGui::Separator();

            bool catchmented = false;
            srand(1);

            if (ImGui::Button("Catch4")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->realTile, K4);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch4 immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->realTile, K4);
            }
            if (ImGui::Button("Catch4 HM")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->realTile, K4_HARD_MIN);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch4 HM immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->realTile, K4_HARD_MIN);
            }
            if (ImGui::Button("Catch4 heightwise")) {
                catchmented = true;
                Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K4);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch4 HM heightwise")) {
                catchmented = true;
                Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K4_HARD_MIN);
            }

            ImGui::Separator();

            if (ImGui::Button("Catch8")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->realTile, K8);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch8 immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->realTile, K8);
            }
            if (ImGui::Button("Catch8 HM")) {
                catchmented = true;
                Catchmenter__color_all(cw->app->realTile, K8_HARD_MIN);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch8 HM immediate")) {
                catchmented = true;
                Catchmenter__color_all_immediate(cw->app->realTile, K8_HARD_MIN);
            }
            if (ImGui::Button("Catch8 heightwise")) {
                catchmented = true;
                Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K8);
            }
            ImGui::SameLine();
            if (ImGui::Button("Catch8 HM heightwise")) {
                catchmented = true;
                Catchmenter__color_all_immediate_heightwise(cw->app->realTile, K8_HARD_MIN);
            }

            if (catchmented) {
                RealTile__texture_generate(cw->app->realTile);
            }

        }
        ImGui::PopID();

    }
    ImGui::End();
}

bool ControlWidget__terminate(ControlWidget* cw) {
    return true;
}

void ControlWidget__destroy(ControlWidget* cw) {
    free(cw);
}
