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
    int width, height;
} Texture;

/**
 * Load texture and register in OpenGL
 * @param filename path to texture's file
 * @return loaded texture
 * @memberof Texture
 */
Texture rm_load_texture(const char* filename);

/**
 * Load texture from memory buffer and register in OpenGL
 * @param [in] data memory buffer to register
 * @param [in] width buffer's width
 * @param [in] height buffer's height
 * @param [in] channels_count number of color channels (ex. RGB = 3, RGBA = 4)
 * @return loaded texture
 * @memberof Texture
 */
Texture rm_texture_from_memory(const void* data, int width, int height, int channels_count);

/**
 * Free texture from OpenGL
 * @param tex Texture to be freed
 * @memberof Texture
 */
void rm_free_texture(Texture tex);


#endif //CATCHEA_RESOURCE_MANAGER_H
