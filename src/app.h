/**
 * @file app.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_APP_H
#define CATCHEA_APP_H

#include <stdio.h>
#include <stdlib.h>

struct GLFWwindow;

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

struct GeoTile;
struct DisTile;

struct MenuWidget;
struct MapWidget;
struct ControlWidget;
struct FooterWidget;

#define APP_NAME "Catchea"

struct App {

    GLFWwindow* window;

    GeoTile* geoTile;
    DisTile* disTile;

    MenuWidget* menuWidget;
    MapWidget* mapWidget;
    ControlWidget* controlWidget;
    FooterWidget* footerWidget;
};

/**
 * Create new App instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to App
 * @memberof App
 */
App* App__create();

/**
 * Initialize application
 * @param app App to be initialized
 * @return true if initialization was successful
 * @memberof App
 */
bool App__init(App* app);

/**
 * Start application
 * @param app App to be started
 * @memberof App
 */
void App__run(App* app);

/**
 * Render application
 * @param app App to be rendered
 * @memberof App
 */
void App__render(App* app);

/**
 * Handle request from MenuWidget to open file
 * @param app App to receive request
 * @param filename file's path
 */
void App_file_open_request(App* app, const char* filename);

/**
 * Terminate application
 * @param app App to be terminated
 * @return true if termination was successful
 * @memberof App
 */
bool App__terminate(App* app);

/**
 * Destroy application
 * @param [in] app App to be destroyed
 * @memberof App
 */
void App__destroy(App* app);

#endif //CATCHEA_APP_H
