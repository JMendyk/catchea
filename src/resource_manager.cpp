/**
 * @file resource_manager.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include "resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture rm_load_texture(const char* filename) {
    /*
     * Based on: https://r3dux.org/tag/ilutglloadimage/
     * Modified to use stb_image instead of DevIL
     */

    Texture loaded_texture;

    int channels_count;
    unsigned char *data = stbi_load(filename, &loaded_texture.widget, &loaded_texture.height, &channels_count, STBI_rgb_alpha);

    if(data == nullptr) {
        fprintf(stderr, "Image load failed - stb_image reports error: %s\n", stbi_failure_reason());
        exit(-1);
    }

    glGenTextures(1, &loaded_texture.texture_id);

    GLuint target = GL_TEXTURE_2D;

    // Bind the texture to a name
    glBindTexture(target, loaded_texture.texture_id);

    // Set texture clamping method
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Set texture interpolation method to use linear interpolation (no MIPMAPS)
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(target, 0, GL_RGBA,
                 loaded_texture.widget,
                 loaded_texture.height,
                 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    // Because we have already copied image data into texture data we can release memory used by image.
    stbi_image_free(data);

    return loaded_texture;
}

void rm_free_texture(Texture tex) {
    glDeleteTextures(1, &tex.texture_id);
}
