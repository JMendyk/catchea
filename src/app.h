/**
 * @file app.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_APP_H
#define CATCHEA_APP_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <gui/menu_widget.h>
#include <gui/control_widget.h>
#include <gui/footer_widget.h>

#define APP_NAME "Catchea"

typedef struct {

    GLFWwindow* window;

    MenuWidget* menuWidget;
    ControlWidget* controlWidget;
    FooterWidget* footerWidget;
} App;

/**
 * Create new App instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to App
 */
App* App__create();

/**
 * Initialize application
 * @param app App to be initialized
 * @return true if initialization was successful
 */
bool App__init(App* app);

/**
 * Start application
 * @param app App to be started
 */
void App__run(App* app);

/**
 * Render application
 * @param app App to be rendered
 */
void App__render(App* app);

/**
 * Terminate application
 * @param app App to be terminated
 * @return true if termination was successful
 */
bool App__terminate(App* app);

/**
 * Destroy application
 * @param [in] app App to be destroyed
 */
void App__destroy(App* app);

#endif //CATCHEA_APP_H
