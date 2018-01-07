/**
 * @file main.cpp
 * @author JMendyk
 * @date 05.01.18
 */

#include "app.h"



int main(int argc, char *argv[]) {

    App* app = App__create();

    if(!app) return 0;
    App__run(app);

    App__destroy(app);

    return 0;
}
