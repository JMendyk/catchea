/**
 * @file menu_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include <app.h>
#include "app.h"

#include "menu_widget.h"
#include "map_widget.h"

MenuWidget* MenuWidget__create() {
    MenuWidget* mw = (MenuWidget*) malloc(sizeof(MenuWidget));
    if(!mw) return mw;

    return mw;
}

bool MenuWidget__init(MenuWidget* mw, struct App* app) {
    mw->app = app;
    mw->btn_tex_open = rm_load_texture("res/assets/ic_folder_open_black_48dp.png");
    mw->btn_tex_tile = rm_load_texture("res/assets/ic_border_outer_black_48dp.png");
    //mw->btn_tex_topo = rm_load_texture("res/assets/ic_terrain_black_48dp.png");
    //mw->btn_tex_catch = rm_load_texture("res/assets/ic_opacity_black_48dp.png");

    return true;
}

void MenuWidget__render(MenuWidget* mw, const ImVec2& window_pos, const ImVec2& window_size, const ImVec2& popup_pos, const ImVec2& popup_size) {
    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Menu Widget", NULL, window_flags);
    {
        ImGui::SetWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetWindowSize(window_size, ImGuiCond_Always);

        const int sz = (int)window_size.y - 16;

        const bool btn_open = ImGui::ImageButton((ImTextureID)mw->btn_tex_open.texture_id, ImVec2(sz, sz), ImVec2(0, 0), ImVec2(1, 1), 0);

        static ImGuiFs::Dialog dlg;

        const char* filename = dlg.chooseFileDialog(btn_open, NULL, ".hgt", NULL, popup_size, popup_pos);
        if(strlen(filename) > 0) {
            fprintf(stderr, "File picked: %s\n", filename);
            App_file_open_request(mw->app, filename);
        }

        ImGui::SameLine();
        ImGui::VerticalSeparator();
        ImGui::SameLine();

        ImGui::ImageButton((ImTextureID)mw->btn_tex_tile.texture_id, ImVec2(sz, sz), ImVec2(0, 0), ImVec2(1, 1), 0);
        ImGui::SameLine();
        ImGui::VerticalSeparator();
        ImGui::SameLine();

        //if(ImGui::ImageButton((ImTextureID)mw->btn_tex_topo.texture_id, ImVec2(sz, sz), ImVec2(0, 0), ImVec2(1, 1), 0)) {
        //    (mw->app)->mapWidget->is_color = 1 - (mw->app)->mapWidget->is_color;
        //    MapWidget__update_tile((mw->app)->mapWidget);
        //}
        //ImGui::SameLine();

        //ImGui::ImageButton((ImTextureID)mw->btn_tex_catch.texture_id, ImVec2(sz, sz), ImVec2(0, 0), ImVec2(1, 1), 0);
        //ImGui::SameLine();
        //
        //ImGui::VerticalSeparator();
        //ImGui::SameLine();
    }
    ImGui::End();
}

bool MenuWidget__terminate(MenuWidget* mw) {
    rm_free_texture(mw->btn_tex_open);
    rm_free_texture(mw->btn_tex_tile);
    //rm_free_texture(mw->btn_tex_topo);
    //rm_free_texture(mw->btn_tex_catch);

    return true;
}

void MenuWidget__destroy(MenuWidget* mw) {
    free(mw);
}
