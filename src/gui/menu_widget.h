/**
 * @file menu_widget.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_MENU_WIDGET_H
#define CATCHEA_MENU_WIDGET_H

#include <stdlib.h>

#include <imgui.h>

typedef struct {

} MenuWidget;

/**
 * Create new MenuWidget instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to MenuWidget
 */
MenuWidget* MenuWidget__create();

/**
 * Initialize MenuWidget at the start of the application
 * @remark Should only be called by App
 * @param mw MenuWidget to be initialized
 * @return true if initialization was successful
 */
bool MenuWidget__init(MenuWidget* mw);

/**
 * Render MenuWidget
 * @param mw MenuWidget to be rendered
 * @param [in] window_pos position of MenuWidget window
 * @param [in] window_size size of MenuWidget window
 */
void MenuWidget__render(MenuWidget* mw, const ImVec2& window_pos, const ImVec2& window_size);

/**
 * Terminate MenuWidget before the application terminates
 * @remark Should only be called by App
 * @param mw MenuWidget to be terminated
 * @return true if termination was successful
 */
bool MenuWidget__terminate(MenuWidget* mw);

/**
 * Destroy MenuWidget
 * @param [in] mw MenuWidget to be destroyed
 */
void MenuWidget__destroy(MenuWidget* mw);

#endif //CATCHEA_MENU_WIDGET_H
