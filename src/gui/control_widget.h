/**
 * @file control_widget.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_CONTROL_WIDGET_H
#define CATCHEA_CONTROL_WIDGET_H

#include <stdlib.h>

#include <imgui.h>

typedef struct {

} ControlWidget;

/**
 * Create new ControlWidget instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to ControlWidget
 */
ControlWidget* ControlWidget__create();

/**
 * Initialize ControlWidget at the start of the application
 * @remark Should only be called by App
 * @param cw ControlWidget to be initialized
 * @return true if initialization was successful
 */
bool ControlWidget__init(ControlWidget* cw);

/**
 * Render ControlWidget
 * @param cw ControlWidget to be rendered
 * @param [in] window_pos position of ControlWidget window
 * @param [in] window_size size of ControlWidget window
 */
void ControlWidget__render(ControlWidget* cw, const ImVec2& window_pos, const ImVec2& window_size);

/**
 * Terminate ControlWidget before the application terminates
 * @remark Should only be called by App
 * @param cw ControlWidget to be terminated
 * @return true if termination was successful
 */
bool ControlWidget__terminate(ControlWidget* cw);

/**
 * Destroy ControlWidget
 * @param [in] cw ControlWidget to be destroyed
 */
void ControlWidget__destroy(ControlWidget* cw);

#endif //CATCHEA_CONTROL_WIDGET_H
