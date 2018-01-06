/**
 * @file footer_widget.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_FOOTER_WIDGET_H
#define CATCHEA_FOOTER_WIDGET_H

#include <stdlib.h>

#include <imgui.h>

typedef struct {

} FooterWidget;

/**
 * Create new FooterWidget instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to FooterWidget
 */
FooterWidget* FooterWidget__create();

/**
 * Initialize FooterWidget at the start of the application
 * @remark Should only be called by App
 * @param fw FooterWidget to be initialized
 * @return true if initialization was successful
 */
bool FooterWidget__init(FooterWidget* fw);

/**
 * Render FooterWidget
 * @param fw FooterWidget to be rendered
 * @param [in] window_pos position of FooterWidget window
 * @param [in] window_size size of FooterWidget window
 */
void FooterWidget__render(FooterWidget* fw, const ImVec2& window_pos, const ImVec2& window_size);

/**
 * Terminate FooterWidget before the application terminates
 * @remark Should only be called by App
 * @param fw FooterWidget to be terminated
 * @return true if termination was successful
 */
bool FooterWidget__terminate(FooterWidget* fw);

/**
 * Destroy FooterWidget
 * @param [in] fw FooterWidget to be destroyed
 */
void FooterWidget__destroy(FooterWidget* fw);

#endif //CATCHEA_FOOTER_WIDGET_H
