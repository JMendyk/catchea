/**
 * @file app.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "app.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <real_tile/hgt_plugin.h>

#include "gui/menu_widget.h"
#include "gui/map_widget.h"
#include "gui/control_widget.h"
#include "gui/footer_widget.h"

#include "dis_interpreters/topographer.h"

App* App__create() {
    App* app = (App*) malloc(sizeof(App));
    if(!app) return app;

    app->menuWidget = MenuWidget__create();
    if(!app->menuWidget)
        return NULL;

    app->mapWidget = MapWidget__create();
    if(!app->mapWidget)
        return NULL;

    app->controlWidget = ControlWidget__create();
    if(!app->controlWidget)
        return NULL;

    app->footerWidget = FooterWidget__create();
    if(!app->footerWidget)
        return NULL;

    return app;
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}

bool App__init(App* app) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    app->window = glfwCreateWindow(1280, 720, APP_NAME, NULL, NULL);
    glfwMakeContextCurrent(app->window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(app->window, true);

    /*
         * Add default app font
         * This must happen before initialization of any other app components
         * since default app font must be defined as the first one.
         */
    ImGuiIO& io = ImGui::GetIO();
    static ImFontConfig defaultFontConfig = ImFontConfig();
    io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Medium.ttf", 15.0f, &defaultFontConfig);
    io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Medium.ttf", 20.0f, &defaultFontConfig);

    ImGui::GetStyle().WindowRounding = 0.0f;

    if(!MenuWidget__init(app->menuWidget, app))
        return false;

    if(!MapWidget__init(app->mapWidget, app))
        return false;

    if(!ControlWidget__init(app->controlWidget, app))
        return false;

    if(!FooterWidget__init(app->footerWidget, app))
        return false;

    return true;
}

void App__run(App* app) {
    if(!App__init(app)) {
        fprintf(stderr, "Failed to initialize the app.\n");
        return;
    }

    while (!glfwWindowShouldClose(app->window)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        App__render(app);

        int display_w, display_h;
        glfwGetFramebufferSize(app->window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(app->window);
    }

    if(!App__terminate(app)) {
        fprintf(stderr, "Failed to properly terminate the app.\n");
        return;
    }
}

void App__render(App* app) {
    int canvas_w, canvas_h;
    glfwGetFramebufferSize(app->window, &canvas_w, &canvas_h);

    static int ui_margin = 16;
    static int menu_widget_height = 64;
    static int footer_widget_height = 32;
    static int control_widget_width = 300;

    /*
     * Map Widget
     */
    const ImVec2 map_widget_pos = ImVec2(
        ui_margin,
        ui_margin + menu_widget_height + ui_margin
    );
    const ImVec2 map_widget_size = ImVec2(
        canvas_w - 2*ui_margin - control_widget_width - ui_margin,
        canvas_h - (ui_margin + menu_widget_height + ui_margin + 0 + ui_margin + footer_widget_height)
    );
    MapWidget__render(app->mapWidget, map_widget_pos, map_widget_size);

    /*
     * Menu Widget
     */
    const ImVec2 menu_widget_pos = ImVec2(
            ui_margin,
            ui_margin
    );
    const ImVec2 menu_widget_size = ImVec2(
            canvas_w - 2*ui_margin - control_widget_width - ui_margin,
            menu_widget_height
    );
    MenuWidget__render(app->menuWidget, menu_widget_pos, menu_widget_size, map_widget_pos, map_widget_size);

    /*
     * Control Widget
     */
    const ImVec2 control_widget_pos = ImVec2(
        canvas_w - ui_margin - control_widget_width,
        ui_margin
    );
    const ImVec2 control_widget_size = ImVec2(
        control_widget_width,
        canvas_h - (ui_margin + footer_widget_height + ui_margin)
    );
    ControlWidget__render(app->controlWidget, control_widget_pos, control_widget_size);

    /*
     * Footer Widget
     */
    const ImVec2 footer_widget_pos = ImVec2(
        0,
        canvas_h - (footer_widget_height)
    );
    const ImVec2 footer_widget_size = ImVec2(
        canvas_w,
        footer_widget_height
    );
    FooterWidget__render(app->footerWidget, footer_widget_pos, footer_widget_size);

}

void App_file_open_request(App* app, const char* filename) {
    //app->geoTile = GeoTile__from_hgt_file(filename);
    //
    //MapWidget__update_tile(app->mapWidget);

    RealTile__destroy(app->realTile);
    app->realTile = RealTile__from_hgt_file(filename);
    RealTile__apply_default_coloring(app->realTile);
}

bool App__terminate(App* app) {
    if(!MenuWidget__terminate(app->menuWidget))
        return false;

    if(!MapWidget__terminate(app->mapWidget))
        return false;

    if(!ControlWidget__terminate(app->controlWidget))
        return false;

    if(!FooterWidget__terminate(app->footerWidget))
        return false;

    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return true;
}

void App__destroy(App* app) {
    MenuWidget__destroy(app->menuWidget);
    MapWidget__destroy(app->mapWidget);
    ControlWidget__destroy(app->controlWidget);
    FooterWidget__destroy(app->footerWidget);

    free(app);
}
