/**
 * @file resource_manager.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_RESOURCE_MANAGER_H
#define CATCHEA_RESOURCE_MANAGER_H

#include <stdlib.h>

#include <GL/gl.h>

typedef struct {
    GLuint texture_id;
    int widget, height;
} Texture;

Texture rm_load_texture(const char* filename);


#endif //CATCHEA_RESOURCE_MANAGER_H
