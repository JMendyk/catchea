/**
 * @file resource_manager.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_RESOURCE_MANAGER_H
#define CATCHEA_RESOURCE_MANAGER_H

#include <stdlib.h>

#include <GL/gl.h>

/**
 * @brief Represents texture with dimensions and id in OpenGL
 * @details
 * For example, to store UI icons/assets
 */
typedef struct {
    GLuint texture_id;
    int widget, height;
} Texture;

/**
 * Load texture and register in OpenGL
 * @param filename path to texture's file
 * @return loaded texture
 * @memberof Texture
 */
Texture rm_load_texture(const char* filename);

/**
 * Free texture from OpenGL
 * @param tex Texture to be freed
 * @memberof Texture
 */
void rm_free_texture(Texture tex);


#endif //CATCHEA_RESOURCE_MANAGER_H
