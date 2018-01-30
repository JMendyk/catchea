/**
 * @file main.cpp
 * @author JMendyk
 * @date 05.01.18
 */

/**
 * @mainpage Catchea - Topographic Data Browser
 *
 * \section intro Introduction
 * This documentation provides information about Catchea's source code
 * to read initial and final documentation (in Polish) go <a href="https://github.com/JMendyk/catchea/blob/master/demo/catchea_pre_documentation.pdf">here</a>.
 */

#define NDEBUG

#include "app.h"


int main(int argc, char *argv[]) {

    assert(false);

    App* app = App__create();

    if(!app) return 0;
    App__run(app);

    App__destroy(app);

    return 0;
}
