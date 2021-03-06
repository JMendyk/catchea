/**
 * @file map_widget.h
 * @author JMendyk
 * @date 07.01.18
 */

#ifndef CATCHEA_MAP_WIDGET_H
#define CATCHEA_MAP_WIDGET_H

#include <cstdlib>

#include <imgui.h>
#include <vector>

#include "resource_manager.h"

struct App;

struct MapWidget {

    App* app;

};

/**
 * Create new MapWidget instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to MapWidget
 * @memberof MapWidget
 */
MapWidget* MapWidget__create();

/**
 * Initialize MapWidget at the start of the application
 * @remark Should only be called by App
 * @param mw MapWidget to be initialized
 * @param app
 * @return true if initialization was successful
 * @memberof MapWidget
 */
bool MapWidget__init(MapWidget* mw, App* app);

/**
 * Render MapWidget
 * @param mw MapWidget to be rendered
 * @param [in] window_pos position of MapWidget window
 * @param [in] window_size size of MapWidget window
 * @memberof MapWidget
 */
void MapWidget__render(MapWidget* mw, const ImVec2& window_pos, const ImVec2& window_size);

/**
 * Terminate MapWidget before the application terminates
 * @remark Should only be called by App
 * @param mw MapWidget to be terminated
 * @return true if termination was successful
 * @memberof MapWidget
 */
bool MapWidget__terminate(MapWidget* mw);

/**
 * Destroy MapWidget
 * @param [in] mw MapWidget to be destroyed
 * @memberof MapWidget
 */
void MapWidget__destroy(MapWidget* mw);

#endif //CATCHEA_MAP_WIDGET_H
