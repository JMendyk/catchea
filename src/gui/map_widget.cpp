/**
 * @file map_widget.cpp
 * @author JMendyk
 * @date 07.01.18
 */

#include <cmath>
#include <cstdio>
#include <dis_interpreters/catchmenter.h>
#include <resource_manager.h>
#include "app.h"

#include "map_widget.h"
#include "dis_interpreters/topographer.h"

#include <string>

#include <ctime>
#include <utils.h>
#include "real_tile/hgt_plugin.h"
#include "control_widget.h"

MapWidget* MapWidget__create() {
    MapWidget* mw = (MapWidget*) malloc(sizeof(MapWidget));
    if(!mw) return mw;

    return mw;
}

inline bool loadRealTile(MapWidget* mw, const int &lat_min, const int &lon_min, const int &lat_max, const int &lon_max) {
    START_BENCH(map_load)

    mw->app->realTile = RealTile__from_hgt_file_batch("res/assets/tiles", lat_min, lon_min, lat_max, lon_max);
    if(!mw->app->realTile)
        return false;

    RealTile__apply_default_coloring(mw->app->realTile);

    STOP_BENCH(map_load)

    fprintf(stderr, "RealTile total load time: %.2lf\n", GET_BENCH(map_load));
}

bool MapWidget__init(MapWidget* mw, App* app) {
    mw->app = app;

    srand(time(NULL));

    //int lat = rand() % (54-49+1) + 49;
    //int lon = rand() % (23-14+1) + 14;
    //
    //loadRealTile(mw, lat, lon, lat, lon);

    return true;
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const float& rat) { return ImVec2(lhs.x*rat, lhs.y*rat); }
static inline ImVec2 operator*(const float& rat, const ImVec2& rhs) { return ImVec2(rhs.x*rat, rhs.y*rat); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x*rhs.x, lhs.y*rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const float& rat) { return ImVec2(lhs.x/rat, lhs.y/rat); }
static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x/rhs.x, lhs.y/rhs.y); }

void MapWidget__render(MapWidget* mw, const ImVec2& window_pos, const ImVec2& window_size) {
    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_AlwaysHorizontalScrollbar
        | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Map Widget", NULL, window_flags);
    {
        ImGui::SetWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetWindowSize(window_size, ImGuiCond_Always);

        if(mw->app->realTile != NULL) {

            ImVec2 scrolling = ImVec2(0.0f, 0.0f);
            const ImVec2 scroll_bound1 = ImVec2((ImGui::GetContentRegionAvail().y) / mw->app->realTile->tex->height,
                                                (ImGui::GetContentRegionAvail().y) / mw->app->realTile->tex->height);
            const ImVec2 scroll_bound2 = ImVec2((ImGui::GetContentRegionAvail().x) / mw->app->realTile->tex->width,
                                                (ImGui::GetContentRegionAvail().x) / mw->app->realTile->tex->width);
            const ImVec2 scroll_bound = scroll_bound1.x > scroll_bound2.x ? scroll_bound1 : scroll_bound2;
            static ImVec2 scale = scroll_bound;
            static bool is_dragging = false;

            ImVec2 initial_scale = scale;

            ImVec2 image_size = ImVec2(mw->app->realTile->tex->width * scale.x,
                                       mw->app->realTile->tex->height * scale.y);

            ImGui::Image((ImTextureID) mw->app->realTile->tex->texture_id, image_size);

            if ((ImGui::IsWindowHovered() || is_dragging) && !ImGui::IsAnyItemActive()) {
                if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))) {
                    scrolling.x += 16;
                }

                if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow))) {
                    scrolling.x -= 16;
                }

                if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
                    scrolling.y += 16;
                }

                if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
                    scrolling.y -= 16;
                }

                if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_PageUp))) {
                    scale = scale + scale / 16.0;
                }

                if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_PageDown))) {
                    scale = scale - scale / 16.0;
                }

                // Scroll using middle mouse button
                if (ImGui::IsMouseDragging(2, 0.0f)) {
                    scrolling = scrolling + ImGui::GetIO().MouseDelta;
                    is_dragging = true;
                }

                if (ImGui::IsMouseReleased(2))
                    is_dragging = false;

                if (ImGui::GetIO().KeyCtrl && ImGui::GetIO().MouseWheel != 0) {
                    float change = (ImGui::GetIO().MouseWheel / fabsf(ImGui::GetIO().MouseWheel)) * 1.05f;
                    if (change < 0)
                        change = -1.0f / change;
                    scale = scale * ImVec2(change, change);
                } else {
                    static float wheelSpeed = 64;
                    if (!ImGui::GetIO().KeyShift) {
                        // Scroll vertically using mouse wheel
                        scrolling = scrolling + ImVec2(0, ImGui::GetIO().MouseWheel) * wheelSpeed;
                    } else {
                        // Scroll horizontally using Shift + mouse wheel
                        scrolling = scrolling + ImVec2(ImGui::GetIO().MouseWheel, 0) * wheelSpeed;
                    }
                }

            }

            scale.x = fmaxf(scale.x, scroll_bound.x);
            scale.y = fmaxf(scale.y, scroll_bound.y);

            ImVec2 center_point = (ImGui::GetIO().MousePos - ImGui::GetWindowPos());

            ImVec2 old_offset = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());
            ImVec2 old_offset_centered = old_offset + center_point;

            ImVec2 scale_change = scale / initial_scale;

            ImVec2 new_offset_centered = old_offset_centered * scale_change;

            ImVec2 new_offset = ImGui::IsWindowHovered() ? (new_offset_centered - center_point - scrolling)
                                                         : old_offset;

            ImGui::SetScrollX(new_offset.x);
            ImGui::SetScrollY(new_offset.y);

            ImVec2 CELL_SIZE = ImVec2(image_size.x / mw->app->realTile->tex->width,
                                      image_size.y / mw->app->realTile->tex->height);

            if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() &&
                ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Z))) {
                // Updates existing DisTile instead of creating new one
                ImVec2 cord = ImGui::GetIO().MousePos - ImGui::GetWindowPos() - ImGui::GetWindowContentRegionMin();

                ImVec2 pos = ImVec2((int) floor(cord.x / CELL_SIZE.x), (int) floor(cord.y / CELL_SIZE.y));
                int x = (int) pos.x;
                int y = (int) pos.y;

                ImGui::BeginTooltip();
                ImGui::Text("%d, %d, %d\n", x, y, mw->app->realTile->heights[y * mw->app->realTile->width + x]);
                ImGui::Text("min: norm, HM");
                ImGui::Text("K4: %d, %d\n", is_local_minimum(mw->app->realTile, x, y, K4),
                            is_local_minimum(mw->app->realTile, x, y, K4_HARD_MIN));
                ImGui::Text("K8: %d, %d\n", is_local_minimum(mw->app->realTile, x, y, K8),
                            is_local_minimum(mw->app->realTile, x, y, K8_HARD_MIN));
                ImGui::EndTooltip();
            }

            if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseReleased(0)) {
                srand(1);
                // Updates existing DisTile instead of creating new one
                ImVec2 cord = ImGui::GetIO().MousePos - ImGui::GetWindowPos() - ImGui::GetWindowContentRegionMin();

                cord.x = cord.x / image_size.x * mw->app->realTile->tex->width;
                cord.y = cord.y / image_size.y * mw->app->realTile->tex->height;

                bool* visi_matrix = (bool*) calloc(mw->app->realTile->height * mw->app->realTile->width, sizeof(bool));

                Catchmenter__from(mw->app->realTile, visi_matrix, (int) cord.x, (int) cord.y,
                                  ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)) ? K4 : K4_HARD_MIN,
                                  mw->app->controlWidget->jumpMax);

                free(visi_matrix);
                RealTile__texture_generate(mw->app->realTile);
            }

            if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseReleased(1)) {
                srand(1);
                // Updates existing DisTile instead of creating new one
                ImVec2 cord = ImGui::GetIO().MousePos - ImGui::GetWindowPos() - ImGui::GetWindowContentRegionMin();

                cord.x = cord.x / image_size.x * mw->app->realTile->tex->width;
                cord.y = cord.y / image_size.y * mw->app->realTile->tex->height;

                bool* visi_matrix = (bool*) calloc(mw->app->realTile->height * mw->app->realTile->width, sizeof(bool));

                Catchmenter__from(mw->app->realTile, visi_matrix, (int) cord.x, (int) cord.y,
                                  ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)) ? K8 : K8_HARD_MIN,
                                  mw->app->controlWidget->jumpMax);

                free(visi_matrix);
                RealTile__texture_generate(mw->app->realTile);
            }

        } else {

            const char* text = "Load tile data to view it.";

            ImVec2 text_size = ImGui::CalcTextSize(text, text + strlen(text));

            fprintf(stderr, "%.02f %.02f\n", text_size.x, text_size.y);

            ImGui::SetCursorPos((window_size - text_size)/2.0);
            ImGui::Text(text);
        }

    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

bool MapWidget__terminate(MapWidget* mw) {

    if(mw->app->realTile != NULL)
        RealTile__destroy(mw->app->realTile);

    return true;
}

void MapWidget__destroy(MapWidget* mw) {
    free(mw);
}
