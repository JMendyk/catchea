/**
 * @file menu_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include <app.h>
#include <real_tile/hgt_plugin.h>
#include "app.h"

#include "menu_widget.h"
#include "map_widget.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

MenuWidget* MenuWidget__create() {
    MenuWidget* mw = (MenuWidget*) malloc(sizeof(MenuWidget));
    if(!mw) return mw;

    return mw;
}

bool MenuWidget__init(MenuWidget* mw, struct App* app) {
    mw->app = app;
    mw->btn_tex_open = rm_load_texture("res/assets/ic_folder_open_black_48dp.png");
    mw->btn_tex_tile = rm_load_texture("res/assets/ic_border_outer_black_48dp.png");
    mw->btn_tex_export = rm_load_texture("res/assets/ic_camera_alt_black_48dp.png");

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


        bool open = true;

        if(ImGui::ImageButton((ImTextureID)mw->btn_tex_tile.texture_id, ImVec2(sz, sz), ImVec2(0, 0), ImVec2(1, 1), 0)) {
            fprintf(stderr, "open %d, ", open);
            ImGui::OpenPopup("Load multiple tiles");
            fprintf(stderr, "%d\n", open);
        }

        ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(popup_size, ImGuiCond_Always);

        if (ImGui::BeginPopupModal("Load multiple tiles", &open)) {
            ImGui::TextWrapped(
                    "This utility helps load multiple tiles and join them to single tile.\n"
                    "Please, provide path to folder with HGT files that have format like \"N49E014.hgt\" "
                    "(north/south, latitude, east/west, longitude). "
                    "Then enter latitude and longitude of lower-left and upper-right corners of rectangle of tiles to load."
            );

            ImGui::Separator();

            float width = ImGui::GetContentRegionAvailWidth()*2/3;

            static char* chosen_folder = (char*) calloc(1, sizeof(char));
            ImGui::Text("Folder with tile: ");
            ImGui::PushID("chosen_folder");
            ImGui::PushItemWidth(width*2/3);
            ImGui::InputText("", chosen_folder, strlen(chosen_folder), ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();

            float button_margin = ImGui::GetStyle().ItemSpacing.x;
            const bool btn_choose = ImGui::Button("Choose", ImVec2(width*1/3 - button_margin, 0));
            ImGui::PopID();

            static ImGuiFs::Dialog tiling_dialog;
            const char* tiling_folder = tiling_dialog.chooseFolderDialog(btn_choose, NULL, "Select folder with HGT files", popup_size, popup_pos, 1.0f);

            if(strlen(tiling_folder) > 0) {
                if(chosen_folder != NULL)
                    free(chosen_folder);
                chosen_folder = (char*) calloc(strlen(tiling_folder)+1, sizeof(char));
                strcpy(chosen_folder, tiling_folder);
            }


            ImGui::Text("Lat/lon of lower-left corner:");

            static int min_cord[2];
            static int max_cord[2];



            ImGui::PushID("min");
            ImGui::PushItemWidth(width);
            ImGui::InputInt2("", min_cord);
            ImGui::PopID();

            ImGui::Text("Lat/lon of upper-right corner:");

            ImGui::PushID("max");
            ImGui::PushItemWidth(width);
            ImGui::InputInt2("", max_cord);
            ImGui::PopID();

            static int error = 0;

            if (ImGui::Button("Load", ImVec2(width, 0))) {
                error = 0;

                if (!ImGuiFs::DirectoryExists(chosen_folder)) {
                    error |= 1 << 0;
                }

                if (!(min_cord[0] <= max_cord[0] && min_cord[1] <= max_cord[1])
                   || !(-90 <= min_cord[0] && min_cord[0] <= 90)
                   || !(-90 <= max_cord[0] && max_cord[0] <= 90)
                   || !(-180 <= min_cord[1] && min_cord[1] <= 180)
                   || !(-180 <= max_cord[1] && max_cord[1] <= 180)) {
                    error |= 1 << 1;
                } else {
                    for (int lat = min_cord[0]; lat <= max_cord[0]; lat++) {
                        for (int lon = min_cord[1]; lon <= max_cord[1]; lon++) {
                            char* full_path = hgt_path_for(chosen_folder, lat, lon);
                            if(!ImGuiFs::FileExists(full_path)) {
                                error |= 1 << 2;
                                break;
                            }
                        }
                    }
                }

                if (error == 0) {
                    mw->app->realTile = RealTile__from_hgt_file_batch(chosen_folder, min_cord[0], min_cord[1], max_cord[0], max_cord[1]);
                    RealTile__apply_default_coloring(mw->app->realTile);
                    ImGui::CloseCurrentPopup();
                }
            }

            if (error) {
                ImVec4 color = ImColor(255, 0, 0).Value;
                ImGui::TextColored(color, "Errors encountered:");
                if (error & 1 << 0) ImGui::TextColored(color, "Provided folder does not exist.");
                if (error & 1 << 1) ImGui::TextColored(color, "Coordinates invalid.");
                if (error & 1 << 2) {
                    for (int lat = min_cord[0]; lat <= max_cord[0]; lat++) {
                        for (int lon = min_cord[1]; lon <= max_cord[1]; lon++) {
                            char* full_path = hgt_path_for(chosen_folder, lat, lon);
                            if(!ImGuiFs::FileExists(full_path)) {
                                char* bad_filename = hgt_path_for("", lat, lon);
                                ImGui::TextColored(color, "File %s does not exist in given folder!", bad_filename);
                            }
                        }
                    }
                }
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if(mw->app->realTile != NULL) {

            ImGui::VerticalSeparator();
            ImGui::SameLine();

            static ImGuiFs::Dialog export_dlg;

            const bool btn_export = ImGui::ImageButton((ImTextureID) mw->btn_tex_export.texture_id, ImVec2(sz, sz),
                                                       ImVec2(0, 0), ImVec2(1, 1), 0);

            const char* export_filename = export_dlg.saveFileDialog(btn_export, NULL, NULL, ".png", NULL, popup_size,
                                                                    popup_pos, 1.0f);

            if (strlen(export_filename) > 0) {
                stbi_write_png(export_filename, mw->app->realTile->width, mw->app->realTile->height, 4,
                               mw->app->realTile->coloring, mw->app->realTile->width * sizeof(RealTile::Coloring));
            }

        }
    }
    ImGui::End();
}

bool MenuWidget__terminate(MenuWidget* mw) {
    rm_free_texture(mw->btn_tex_open);
    rm_free_texture(mw->btn_tex_tile);
    rm_free_texture(mw->btn_tex_export);

    return true;
}

void MenuWidget__destroy(MenuWidget* mw) {
    free(mw);
}
