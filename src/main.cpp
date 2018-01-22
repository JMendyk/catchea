/**
 * @file main.cpp
 * @author JMendyk
 * @date 05.01.18
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
