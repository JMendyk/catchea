/**
 * @file menu_widget.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "menu_widget.h"



MenuWidget* MenuWidget__create() {
    MenuWidget* mw = (MenuWidget*) malloc(sizeof(MenuWidget));
    if(!mw) return mw;

    return mw;
}

bool MenuWidget__init(MenuWidget* mw) {
    return true;
}

void MenuWidget__render(MenuWidget* mw, const ImVec2& window_pos, const ImVec2& window_size) {
    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Menu Widget", NULL, window_flags);
    {
        ImGui::SetWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetWindowSize(window_size, ImGuiCond_Always);
    }
    ImGui::End();
}

bool MenuWidget__terminate(MenuWidget* mw) {
    return true;
}

void MenuWidget__destroy(MenuWidget* mw) {
    free(mw);
}
