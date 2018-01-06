/**
 * @file app.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "app.h"



App* App__create() {
    App* app = (App*) malloc(sizeof(App));
    if(!app) return app;

    app->menuWidget = MenuWidget__create();
    if(!app->menuWidget)
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

    if(!MenuWidget__init(app->menuWidget))
        return false;

    if(!ControlWidget__init(app->controlWidget))
        return false;

    if(!FooterWidget__init(app->footerWidget))
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

}

bool App__terminate(App* app) {
    if(!MenuWidget__terminate(app->menuWidget))
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
    ControlWidget__destroy(app->controlWidget);
    FooterWidget__destroy(app->footerWidget);

    free(app);
}
