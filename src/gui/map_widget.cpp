/**
 * @file map_widget.cpp
 * @author JMendyk
 * @date 07.01.18
 */

#include <math.h>
#include <cstdio>
#include <dis_interpreters/catchmenter.h>
#include <resource_manager.h>
#include "app.h"

#include "map_widget.h"
#include "dis_interpreters/topographer.h"




MapWidget* MapWidget__create() {
    MapWidget* mw = (MapWidget*) malloc(sizeof(MapWidget));
    if(!mw) return mw;

    return mw;
}

bool MapWidget__init(MapWidget* mw, App* app) {
    mw->app = app;
    mw->is_color = 0;

    START_BENCH(map_load)

    //START_BENCH(to_geo_tile)
    //app->geoTile = GeoTile__from_hgt_file_batch("res/assets/tiles", 49, 14, 54, 23);

    int lat = rand() % (54-49+1) + 49;
    int lon = rand() % (23-14+1) + 14;

    app->geoTile = GeoTile__from_hgt_file_batch("res/assets/tiles", lat, lon, lat, lon);

    if(!app->geoTile)
        return false;

    //STOP_BENCH(to_geo_tile)

    //fprintf(stderr, "Loading to GeoTile: %.2lf\n", GET_BENCH(to_geo_tile));

    MapWidget__update_tile(mw, { 000, 000, 000, 255 }, { 255, 000, 000, 255 }, {
        //std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32768),
        //std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32767),
        std::make_pair((DisTileSample){ 000, 000, 000, 255 },      0),
        std::make_pair((DisTileSample){ 255, 255, 255, 255 },   2000)
    });

    STOP_BENCH(map_load)

    fprintf(stderr, "Total load time: %.2lf\n", GET_BENCH(map_load));

    return true;
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const float& rat) { return ImVec2(lhs.x*rat, lhs.y*rat); }
static inline ImVec2 operator*(const float& rat, const ImVec2& rhs) { return ImVec2(rhs.x*rat, rhs.y*rat); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x*rhs.x, lhs.y*rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const float& rat) { return ImVec2(lhs.x/rat, lhs.y/rat); }

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

        ImVec2 scrolling = ImVec2(0.0f, 0.0f);
        const ImVec2 scroll_bound = ImVec2((ImGui::GetContentRegionAvail().y)/mw->texTile.height, (ImGui::GetContentRegionAvail().y)/mw->texTile.height);
        static ImVec2 scale = scroll_bound;
        static bool is_dragging = false;

        ImVec2 old_offset = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());

        if ((ImGui::IsWindowHovered() || is_dragging) && !ImGui::IsAnyItemActive()) {
            // Scroll using middle mouse button
            if (ImGui::IsMouseDragging(2, 0.0f)) {
                scrolling = scrolling + ImGui::GetIO().MouseDelta;
                is_dragging = true;
            }

            if(ImGui::IsMouseReleased(2))
                is_dragging = false;

            if(ImGui::GetIO().KeyCtrl && ImGui::GetIO().MouseWheel != 0) {
                float change = (ImGui::GetIO().MouseWheel / fabsf(ImGui::GetIO().MouseWheel)) * 1.05f;
                if(change < 0)
                    change = -1.0f/change;
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

        ImVec2 offset = old_offset - scrolling;
        ImGui::SetScrollX(offset.x);
        ImGui::SetScrollY(offset.y);

        scale.x = fmaxf(scale.x, scroll_bound.x);
        scale.y = fmaxf(scale.y, scroll_bound.y);

        ImVec2 image_size = ImVec2(mw->texTile.width * scale.x, mw->texTile.height * scale.y);

        ImGui::Image((ImTextureID)mw->texTile.texture_id, image_size);

        //static ImVec2 cord = ImVec2(2, 2);

        if(ImGui::IsMouseDown(0)) {
            // Updates existing DisTile instead of creating new one
            ImVec2 cord = ImGui::GetIO().MousePos - ImGui::GetWindowPos() - ImGui::GetWindowContentRegionMin();


            rm_free_texture(mw->texTile);

            cord.x = cord.x / image_size.x * mw->texTile.width;
            cord.y = cord.y / image_size.y * mw->texTile.height;

            Catchmenter__color_pixel(mw->app->geoTile, mw->app->disTile, static_cast<int>(cord.x),
                                                        static_cast<int>(cord.y));
            mw->texTile = DisTile__to_texture(mw->app->disTile);
        }

        //ImGui::SetCursorPos(cord);
        //ImGui::Button("Label");


        //ImU32 GRID_COLOR = ImColor(200, 200, 200, 40);
        //
        //ImDrawList *dl = ImGui::GetWindowDrawList();

        //const ImVec2 dl_origin = ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
        //for (float x = 0; x < image_size.x; x += scale.x * 16)
        //    dl->AddLine(ImVec2(x, 0.0f) + dl_origin, ImVec2(x, image_size.y) + dl_origin, GRID_COLOR);
        //for (float y = 0; y < image_size.y; y += scale.y * 16)
        //    dl->AddLine(ImVec2(0.0f, y) + dl_origin, ImVec2(image_size.x, y) + dl_origin, GRID_COLOR);
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void MapWidget__update_tile(MapWidget* mw) {
    if(mw->app->disTile != NULL) {
        rm_free_texture(mw->texTile);
        DisTile__destroy(mw->app->disTile);
    }
    mw->app->disTile = Topographer__interpret(mw->app->geoTile, mw->is_color);
    mw->texTile = DisTile__to_texture(mw->app->disTile);
}

void MapWidget__update_tile(MapWidget* mw, const DisTileSample& lower, const DisTileSample& upper,
                            const std::vector< std::pair<DisTileSample, geo_sample_t> >& steps) {
    if(mw->app->disTile != NULL) {
        rm_free_texture(mw->texTile);
        DisTile__destroy(mw->app->disTile);
    }

    mw->app->disTile = Topographer__interpret_param(mw->app->geoTile, lower, upper, steps);
    mw->texTile = DisTile__to_texture(mw->app->disTile);
}

bool MapWidget__terminate(MapWidget* mw) {
    rm_free_texture(mw->texTile);

    DisTile__destroy(mw->app->disTile);
    GeoTile__destroy(mw->app->geoTile);

    return true;
}

void MapWidget__destroy(MapWidget* mw) {
    free(mw);
}
