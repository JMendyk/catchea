/**
 * @file menu_widget.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_MENU_WIDGET_H
#define CATCHEA_MENU_WIDGET_H

#include <cstdlib>

#include <imgui.h>
#include <imgui_internal.h>

#include "resource_manager.h"

struct App;

struct MenuWidget {

    Texture btn_tex_open, btn_tex_tile, btn_tex_export;

    App* app;

};

/**
 * Create new MenuWidget instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to MenuWidget
 * @memberof MenuWidget
 */
MenuWidget* MenuWidget__create();

/**
 * Initialize MenuWidget at the start of the application
 * @remark Should only be called by App
 * @param mw MenuWidget to be initialized
 * @param app
 * @return true if initialization was successful
 * @memberof MenuWidget
 */
bool MenuWidget__init(MenuWidget* mw, App* app);

/**
 * Render MenuWidget
 * @param mw MenuWidget to be rendered
 * @param [in] window_pos position of MenuWidget window
 * @param [in] window_size size of MenuWidget window
 * @memberof MenuWidget
 */
void MenuWidget__render(MenuWidget* mw, const ImVec2& window_pos, const ImVec2& window_size, const ImVec2& popup_pos, const ImVec2& popup_size);

/**
 * Terminate MenuWidget before the application terminates
 * @remark Should only be called by App
 * @param mw MenuWidget to be terminated
 * @return true if termination was successful
 * @memberof MenuWidget
 */
bool MenuWidget__terminate(MenuWidget* mw);

/**
 * Destroy MenuWidget
 * @param [in] mw MenuWidget to be destroyed
 * @memberof MenuWidget
 */
void MenuWidget__destroy(MenuWidget* mw);

#endif //CATCHEA_MENU_WIDGET_H
